
// This source file was auto-generated by ClassMate++
// Created: 2 May 2020 8:43:52 pm
// Copyright (c) 2020, HurleyWorks

using namespace Microsoft::glTF;
using Eigen::Quaternionf;
using juce::File;
using juce::String;
using sabi::MeshBuffers;
using sabi::MeshBuffersHandle;
using sabi::PixelBuffer;
using sabi::PixelBufferHandle;
using sabi::RenderableData;
using sabi::Surface;
using sabi::TextureHandle;
using sabi::TextureSampler;
using sabi::VertexMap;
using sabi::VertexMapRef;
using sabi::VertexMapType;

void GLTFloader::load (const std::filesystem::path path, LoadData& loadData)
{
    ScopedStopWatch sw ("PARSE GLTF FILE");

    try
    {
        createDocument (path);

        for (const auto& node : document.nodes.Elements())
        {
            RenderableData data;

            Pose pose;
            pose.setIdentity();
            Scale scale = Vector3f::Constant (1.0f);

            TransformationType transType = node.GetTransformationType();

            //  4x4 transform matrix
            if (transType == TransformationType::TRANSFORMATION_MATRIX)
            {
                // is the scale built into the linear() part????
                Matrix4f mat (node.matrix.values.data());
                pose.matrix() = mat;

            } // translation, rotation, scale
            else if (transType == TransformationType::TRANSFORMATION_TRS)
            {
                Quaternionf q (node.rotation.w, node.rotation.x, node.rotation.y, node.rotation.z);
                Vector3f trans (node.translation.x, node.translation.y, node.translation.z);
                Matrix3f linear = q.toRotationMatrix();
                scale = Vector3f (node.scale.x, node.scale.y, node.scale.z);

                pose.linear() = linear;
                pose.translation() = trans;
            }

            data.spacetime.worldTransform = pose;
            data.spacetime.scale = scale;

            // a node might not have a mesh
            if (node.meshId.empty()) continue;

            // get mesh data
            const Microsoft::glTF::Mesh& mesh = document.meshes[node.meshId];
            if (mesh.primitives.size())
                parseGLTFMesh (data, mesh);

            loadData.push_back (data);
        }
    }
    catch (const GLTFException& ex)
    {
        std::stringstream ss;

        ss << "Microsoft::glTF::Deserialize failed: ";
        ss << ex.what();

        throw std::runtime_error (ss.str());
    }
}

void GLTFloader::createDocument (const std::filesystem::path& path)
{
    glTFfolder = path.parent_path();

    // Pass the absolute path, without the filename, to the stream reader
    streamReader = std::make_unique<StreamReader> (path.parent_path());

    std::filesystem::path pathFile = path.filename();
    std::filesystem::path pathFileExt = pathFile.extension();

    auto gltfStream = streamReader->GetInputStream (pathFile.u8string()); // Pass a UTF-8 encoded filename to GetInputString
    auto gltfResourceReader = std::make_unique<GLTFResourceReader> (std::move (streamReader));

    std::stringstream manifestStream;

    // Read the contents of the glTF file into a string using a std::stringstream
    manifestStream << gltfStream->rdbuf();
    std::string manifest = manifestStream.str();

    resourceReader = std::move (gltfResourceReader);

    if (!resourceReader)
    {
        throw std::runtime_error ("path filename extension must be .gltf or .glb");
    }

    document = Deserialize (manifest);
}

void GLTFloader::parseGLTFMesh (RenderableData& data, const Microsoft::glTF::Mesh& mesh)
{
    data.mesh = std::make_shared<MeshBuffers>();

    vertexOffsets.clear();
    vertexOffsets.push_back (0);

    uint32_t totalVertices = getTotalVertexCount (mesh);

    // allocate for all vertices
    data.mesh->V.resize (3, totalVertices);

    // fill in the primitive geometry data
    getVertices (data, mesh);
    getTriangles (data, mesh);
    getTextureCoords_0 (data, mesh);
    getTextureCoords_1 (data, mesh);
    getMaterials (data, mesh);
}

uint32_t GLTFloader::getTotalVertexCount (const Microsoft::glTF::Mesh& mesh)
{
    uint32_t totalVertices = 0;

    for (const auto& meshPrimitive : mesh.primitives)
    {
        std::vector<float> positions = MeshPrimitiveUtils::GetPositions (document, *resourceReader.get(), meshPrimitive);
        if (!positions.size()) continue;

        // positions are stored in Vector3f
        size_t vertexCount = positions.size() / 3;

        totalVertices += vertexCount;

        // keep track of the vertex offset for this primitive
        vertexOffsets.push_back (totalVertices);
    }

    return totalVertices;
}

void GLTFloader::getVertices (RenderableData& data, const Microsoft::glTF::Mesh& mesh)
{
    uint32_t index = 0;

    for (const auto& meshPrimitive : mesh.primitives)
    {
        // get vertex positions
        std::vector<float> positions = MeshPrimitiveUtils::GetPositions (document, *resourceReader.get(), meshPrimitive);
        if (!positions.size()) continue;

        // positions are stored in Vector3f
        size_t vertexCount = positions.size() / 3;
        std::memcpy (data.mesh->V.data() + vertexOffsets[index++] * 3, positions.data(), positions.size() * sizeof (float));
    }
}

void GLTFloader::getTriangles (RenderableData& data, const Microsoft::glTF::Mesh& mesh)
{
    uint32_t index = 0;

    // get triangle indices,  MeshPrimitiveUtils will throw an exception when there aren't any of the data type
    for (const auto& meshPrimitive : mesh.primitives)
    {
        // new Surface for each Primitive
        Surface s;
        uint32_t vertexOffset = vertexOffsets[index++];

        try
        {
            // indicies can be stored in different data types
            std::vector<uint16_t> indices16 = MeshPrimitiveUtils::GetIndices16 (document, *resourceReader.get(), meshPrimitive);
            if (indices16.size())
            {
                size_t triangleCount = indices16.size() / 3;
                MatrixXu16 tris; // triangle indices
                tris.resize (3, triangleCount);
                std::memcpy (tris.data(), indices16.data(), indices16.size() * sizeof (uint16_t));

                // need to cast up to uin32_
                s.indices() = tris.cast<uint32_t>();

                // apply the vertex offset
                MatrixXu& indices = s.indices();
                for (int i = 0; i < indices.cols(); i++)
                {
                    Vector3u tri = indices.col (i);
                    tri.x() += vertexOffset;
                    tri.y() += vertexOffset;
                    tri.z() += vertexOffset;

                    indices.col (i) = tri;
                }
                data.mesh->S.push_back (s);
                continue;
            }
        }
        catch (const GLTFException& ex)
        {
            // indices must be uint32_t
        }

        try
        {
            std::vector<uint32_t> indices32 = MeshPrimitiveUtils::GetIndices32 (document, *resourceReader.get(), meshPrimitive);
            if (indices32.size())
            {
                size_t triangleCount = indices32.size() / 3;
                MatrixXu F; // triangle indices
                F.resize (3, triangleCount);
                std::memcpy (F.data(), indices32.data(), indices32.size() * sizeof (uint32_t));

                s.indices() = F;

                // apply the vertex offset
                MatrixXu& indices = s.indices();
                for (int i = 0; i < indices.cols(); i++)
                {
                    Vector3u tri = indices.col (i);
                    tri.x() += vertexOffset;
                    tri.y() += vertexOffset;
                    tri.z() += vertexOffset;

                    indices.col (i) = tri;
                }

                data.mesh->S.push_back (s);
            }
        }
        catch (const GLTFException& ex)
        {
            // if we get here something is wrong
            std::stringstream ss;

            ss << "Microsoft::glTF::Deserialize failed: ";
            ss << ex.what();

            throw std::runtime_error (ss.str());
        }
    }
}

void GLTFloader::getMaterials (RenderableData& data, const Microsoft::glTF::Mesh& mesh)
{
    uint32_t index = 0;

    for (const auto& meshPrimitive : mesh.primitives)
    {
        const Microsoft::glTF::Material& material = document.materials[meshPrimitive.materialId];

        sabi::Material mat;
        mat.metallic = material.metallicRoughness.metallicFactor;
        mat.roughness = material.metallicRoughness.roughnessFactor;

        Color4 c = material.metallicRoughness.baseColorFactor;
        mat.base_color = Vector4f (c.r, c.g, c.b, c.a);

        for (auto t : material.GetTextures())
        {
            if (!t.first.empty() && t.second == TextureType::BaseColor)
            {
                const Microsoft::glTF::Texture text = document.textures[t.first];
                if (text.imageId.empty()) continue;

                Image image = document.images[text.imageId];

                TextureHandle texture = sabi::Texture::create();
                PixelBufferHandle pixels = PixelBuffer::createHandle();
                pixels->spec.name = BASE_COLOR_TEXTURE_NAME;

                std::filesystem::path path = std::string (image.uri);
                if (std::filesystem::exists(path))
                {
                    File f (String (image.uri));
                    if (f.existsAsFile())
                    {
                        pixels->spec.fullPath = f.getFullPathName().toStdString();
                    }
                }
                else
                {
                    String fullPath = String (glTFfolder.generic_string()) + juce::File::getSeparatorString() + String (image.uri);
                    pixels->spec.fullPath = fullPath.toStdString();
                }

                TextureSampler sampler;
                sampler.imageID = pixels->getID();

                texture->addTextureImage (pixels);
                texture->addTextureSampler (sampler);

                mat.base_color_tex = texture;
            }

            if (!t.first.empty() && t.second == TextureType::MetallicRoughness)
            {
                const Microsoft::glTF::Texture text = document.textures[t.first];
                if (text.imageId.empty()) continue;

                Image image = document.images[text.imageId];

                TextureHandle texture = sabi::Texture::create();
                PixelBufferHandle pixels = PixelBuffer::createHandle();
                pixels->spec.name = METALLIC_ROUGHNESS_TEXTURE_NAME;
                String fullPath = String (glTFfolder.generic_string()) + juce::File::getSeparatorString() + String (image.uri);
                pixels->spec.fullPath = fullPath.toStdString();

                TextureSampler sampler;
                sampler.imageID = pixels->getID();

                texture->addTextureImage (pixels);
                texture->addTextureSampler (sampler);

                mat.metallic_roughness_tex = texture;
            }

            if (!t.first.empty() && t.second == TextureType::Normal)
            {
                const Microsoft::glTF::Texture text = document.textures[t.first];
                if (text.imageId.empty()) continue;

                Image image = document.images[text.imageId];

                TextureHandle texture = sabi::Texture::create();
                PixelBufferHandle pixels = PixelBuffer::createHandle();
                pixels->spec.name = NORMAL_MAP_TEXTURE_NAME;
                String fullPath = String (glTFfolder.generic_string()) + juce::File::getSeparatorString() + String (image.uri);
                pixels->spec.fullPath = fullPath.toStdString();

                TextureSampler sampler;
                sampler.imageID = pixels->getID();

                texture->addTextureImage (pixels);
                texture->addTextureSampler (sampler);

                mat.normal_tex = texture;
            }

            if (!t.first.empty() && t.second == TextureType::Occlusion)
            {
                std::cout << "Occlusion texture ID is " << t.first << std::endl;
            }

            if (!t.first.empty() && t.second == TextureType::Emissive)
            {
                std::cout << "Emissive texture ID is " << t.first << std::endl;
            }
        }

        if (index < data.mesh->S.size())
        {
            Surface& s = data.mesh->S[index++];
            s.setMaterial (mat);
        }
        else
        {
            LOG (CRITICAL) << "Invalid index encountered";
        }
    }
}

void GLTFloader::getTextureCoords_0 (RenderableData& data, const Microsoft::glTF::Mesh& mesh)
{
    // each primitve represents a new surface
    uint32_t surfaceIndex = 0;
    for (const auto& meshPrimitive : mesh.primitives)
    {
        try
        {
            // exception thrown if there are none
            std::vector<float> textCoords = MeshPrimitiveUtils::GetTexCoords_0 (document, *resourceReader.get(), meshPrimitive);

            // textCoords are stored in Vector2f  Is this always true?
            size_t textCoordsCount = textCoords.size() / 2;
            VertexMapRef vmap = VertexMap::create();
            vmap->dim = 2;
            vmap->type = VertexMapType::UV;
            vmap->values.resize (2, textCoordsCount);
            vmap->name = std::string ("UV_") + std::to_string (surfaceIndex);

            std::memcpy (vmap->values.data(), textCoords.data(), textCoords.size() * sizeof (float));

            if (surfaceIndex < data.mesh->S.size())
            {
                Surface& s = data.mesh->S[surfaceIndex];
                s.addVertexMap (vmap);
            }
            //  data.mesh->vmaps.insert (std::make_pair ("UV", vmap));

            LOG (DBUG) << "This mesh has " << textCoordsCount << " UV_0s ";
        }
        catch (const GLTFException& ex)
        {
            // no TextCoords0
            LOG (INFO) << ex.what();
        }

        ++surfaceIndex;
    }
}

void GLTFloader::getTextureCoords_1 (RenderableData& data, const Microsoft::glTF::Mesh& mesh)
{
    // FIXME
    for (const auto& meshPrimitive : mesh.primitives)
    {
    }
}
