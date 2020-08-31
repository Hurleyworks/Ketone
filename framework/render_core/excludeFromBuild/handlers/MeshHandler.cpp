// This source file was auto-generated by ClassMate++
// Created: 9 Aug 2020 3:08:12 pm
// Copyright (c) 2020, HurleyWorks


// much of this code comes from https://github.com/shocker-0x15/OptiX7_Utility

using juce::String;
using sabi::SpaceTime;

void MeshHandler::initialize (MoojiStateRef& state)
{
    this->state = state;
    material.initialize (state);
}

InstanceRef MeshHandler::addRenderableNode (RenderableNode& node)
{
    // instances don't have any geometry of their own
    if (node->isInstance())
        return createInstance (node);

    GeometryInstanceRef geomInst = addRenderableGeometry (node);
    uint32_t materialSetCount = material.addRenderableMaterials (geomInst, node);
    uint32_t gasIndex = createGeometryGroup (geomInst, materialSetCount);

    // FIXME clientID is used in LW so maybe add a new ID  type  to Renderable
    node->setClientID (gasIndex);

    return createInstance (node);
}

void MeshHandler::onSelectionStateChanged (RenderableNode& node)
{
    Instance* const inst = static_cast<Instance*> (node->getUserdata2());
    if (!inst) return;

    uint32_t geoGroupID = node->isInstance() ? node->getInstancedFrom()->getClientID() : node->getClientID();
    const GeometryGroupRef& geomGroup = state->geomGroups.at (geoGroupID);

    // material set 1 is the selection material (orange)
    inst->optixInst.setChild (geomGroup->optixGAS, node->getState().isSelected() ? 1 : 0);
}

GeometryInstanceRef MeshHandler::addRenderableGeometry (RenderableNode& node)
{
    auto mesh = node->getMesh();

    OptixVertices vertices;
    uint32_t vertexCount = addVertices (mesh, vertices);
    if (!vertexCount)
        throw std::runtime_error ("No vertices found: " + node->getName());

    OptixTriangles triangles;
    uint32_t triangleCount = addAllTriangles (mesh, triangles);
    if (!triangleCount)
        throw std::runtime_error ("No triangles found: " + node->getName());

    VertexBufferRef vertexBuffer = make_shared_with_deleter<cudau::TypedBuffer<Shared::Vertex>> (
        [] (cudau::TypedBuffer<Shared::Vertex>* p) {
            p->finalize();
        });
    vertexBuffer->initialize (state->cuContext, g_bufferType, vertices);

    GeometryInstanceRef geomInst = make_shared_with_deleter<GeometryInstance> (GeometryInstance::finalize);
    uint32_t geomInstIndex = state->geometryInstSlotFinder.getFirstAvailableSlot();
    state->geometryInstSlotFinder.setInUse (geomInstIndex);
    geomInst->optixEnv = state.get();
    geomInst->name = node->getName();

    geomInst->vertexBuffer = vertexBuffer;
    geomInst->triangleBuffer.initialize (state->cuContext, g_bufferType, triangles);
    geomInst->geomInstIndex = geomInstIndex;
    geomInst->optixGeomInst = state->scene.createGeometryInstance();
    geomInst->optixGeomInst.setVertexBuffer (&*vertexBuffer);
    geomInst->optixGeomInst.setTriangleBuffer (&geomInst->triangleBuffer);

    Shared::GeometryData geomData = {};
    geomData.vertexBuffer = vertexBuffer->getDevicePointer();
    geomData.triangleBuffer = geomInst->triangleBuffer.getDevicePointer();
    geomInst->optixGeomInst.setUserData (geomData);

    // FIXME 
    if (node->getName() == DEFAULT_AREA_LIGHT_NAME)
        state->areaLightIndex = geomInstIndex;

    state->geomInsts[state->geomInstSerialID++] = geomInst;

    return geomInst;
}

InstanceRef MeshHandler::createInstance (RenderableNode& node)
{
    bool isInstance = node->isInstance();

    CUstream& curCuStream = state->engine.getStream (state->bufferIndex);

    const SpaceTime& st = node->getSpaceTime();
    const Vector3f& pos = st.worldTransform.translation();
    const Vector3f& scale = st.scale;

    // OptixInstance wants a affine object-to-world transformation
    // as 3x4 matrix in row-major layout
    MatrixRowMajor34f t;
    getWorldTransform (t, node->getSpaceTime().worldTransform, node->getSpaceTime().scale);

    uint32_t serialID = state->instSerialID++;
    state->weakNodes.insert (std::make_pair (serialID, node));

    InstanceRef inst = make_shared_with_deleter<Instance> (Instance::finalize);
    inst->node = node;
    inst->optixEnv = state.get();
    inst->name = node->getName();
    inst->optixInst = state->scene.createInstance();
    inst->scale = float3 (scale.x(), scale.y(), scale.z());
    inst->position = float3 (pos.x(), pos.y(), pos.z());
    inst->rollPitchYaw[0] = 0.0f;
    inst->rollPitchYaw[1] = 0.0f;
    inst->rollPitchYaw[2] = 0.0f;

    node->setUserData2 (inst.get());

    // we stored the geoGroupID in node clientID
    // instances use the InstancedFrom's GAS
    uint32_t geoGroupID = isInstance ? node->getInstancedFrom()->getClientID() : node->getClientID();

    const GeometryGroupRef& geomGroup = state->geomGroups.at (geoGroupID);
    inst->geomGroup = geomGroup;
    geomGroup->parentInsts.insert (inst);

    // if motion blur is on then child is optixu::Transform
    if (!state->engine.usingMotionBlur())
    {
        auto randomMaterialSet = RandoM::get<uint8_t> (101, 2);
        inst->optixInst.setChild (geomGroup->optixGAS, isInstance ? randomMaterialSet : 0);
    }
    else
    {
         // Todo motion blur
    }

    inst->optixInst.setTransform (t.data());

    if (!geomGroup->dataTransfered)
    {
        Shared::GASData gasData;
        gasData.preTransforms = geomGroup->preTransformBuffer.getDevicePointer();

        CUDADRV_CHECK (cuMemcpyHtoDAsync (state->gasDataBuffer.getCUdeviceptrAt (geomGroup->gasIndex),
                                          &gasData,
                                          sizeof (gasData),
                                          curCuStream));
        geomGroup->dataTransfered = true;
    }

    state->insts[serialID] = inst;

    return inst;
}

uint32_t MeshHandler::createGeometryGroup (GeometryInstanceRef& geomInst, uint32_t materialSetCount)
{
    CUstream& curCuStream = state->engine.getStream (state->bufferIndex);

    uint32_t serialID = state->gasSerialID++;

    GeometryGroupRef geomGroup = make_shared_with_deleter<GeometryGroup> (GeometryGroup::finalize);
    uint32_t gasIndex = state->gasSlotFinder.getFirstAvailableSlot();
    state->gasSlotFinder.setInUse (gasIndex);
    char name[256];
    sprintf_s (name, "GAS-%u", serialID);
    geomGroup->optixEnv = state.get();
    geomGroup->gasIndex = gasIndex;
    geomGroup->serialID = serialID;
    geomGroup->name = name;
    geomGroup->optixGAS = state->scene.createGeometryAccelerationStructure();
    geomGroup->optixGAS.setConfiguration (optixu::ASTradeoff::PreferFastTrace, false, true, false);

    geomGroup->optixGAS.setNumMaterialSets (materialSetCount);
    for (int matSetIdx = 0; matSetIdx < materialSetCount; ++matSetIdx)
        geomGroup->optixGAS.setNumRayTypes (matSetIdx, state->engine.getRayTypeCount());

    geomGroup->optixGAS.setUserData (gasIndex);
    geomGroup->preTransformBuffer.initialize (state->cuContext, g_bufferType, 1);
    geomGroup->dataTransfered = false;

    geomGroup->geomInsts.push_back (geomInst);
    geomGroup->preTransforms.emplace_back();
    geomGroup->optixGAS.addChild (geomInst->optixGeomInst, geomGroup->preTransformBuffer.getCUdeviceptrAt (0));

    CUDADRV_CHECK (cuMemcpyHtoDAsync (geomGroup->preTransformBuffer.getCUdeviceptr(),
                                      geomGroup->preTransforms.data(),
                                      geomGroup->preTransformBuffer.sizeInBytes(),
                                      curCuStream));

    state->geomGroups[serialID] = geomGroup;

    return serialID;
}

uint32_t MeshHandler::addVertices (MeshBuffersHandle& mesh, OptixVertices& vertices)
{
    uint32_t vertexCount = mesh->V.cols();

    if (!vertexCount)
        return 0;

    vertices.reserve (vertexCount);

    for (int i = 0; i < vertexCount; i++)
    {
        Shared::Vertex v;

        Vector3f p = mesh->V.col (i);
        Vector3f n = mesh->N.col (i);

        // vecStr3f (n, DBUG, "NORMAL");

        v.position.x = p.x();
        v.position.y = p.y();
        v.position.z = p.z();

        v.normal.x = n.x();
        v.normal.y = n.y();
        v.normal.z = n.z();

        v.texCoord.x = 0;
        v.texCoord.y = 0;

        vertices.push_back (v);
    }

    return vertexCount;
}

void MeshHandler::addSurfaceTriangles (MeshBuffersHandle& mesh, OptixSurfaces& optixSurfaces)
{
    optixSurfaces.reserve (mesh->S.size());

    for (const auto& s : mesh->S)
    {
        OptixTriangles triangles;
        triangles.reserve (s.triangleCount());

        const MatrixXu& tris = s.indices();
        for (int i = 0; i < tris.cols(); i++)
        {
            Vector3u tri = tris.col (i);

            Shared::Triangle t;
            t.index0 = tri.x();
            t.index1 = tri.y();
            t.index2 = tri.z();

            triangles.push_back (t);
        }

        optixSurfaces.push_back (std::move (triangles));
    }
}

uint32_t MeshHandler::addAllTriangles (MeshBuffersHandle& mesh, OptixTriangles& triangles)
{
    MatrixXu tris;
    mesh->getAllSurfaceIndices (tris);
    triangles.reserve (tris.cols());

    for (int i = 0; i < tris.cols(); i++)
    {
        Vector3u tri = tris.col (i);

        Shared::Triangle t;
        t.index0 = tri.x();
        t.index1 = tri.y();
        t.index2 = tri.z();

        triangles.push_back (t);
    }

    return tris.cols();
}
