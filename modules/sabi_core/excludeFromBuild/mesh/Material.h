// This header file was auto-generated by ClassMate++
// Created: 18 Mar 2019 8:01:18 pm
// Copyright (c) 2019, HurleyWorks

#pragma once

using Eigen::Vector4f;

using MaterialHandle = std::shared_ptr<struct Material>;

struct Material : public HasId
{
    static MaterialHandle create() { return std::make_shared<Material>(); }
    const MaterialID getID() const { return id(); }

    void addTexture (TextureHandle& texture)
    {
        textureDB.insert (std::make_pair (texture->getID(), texture));
    }

    TextureHandle getTexture (TextureID key)
    {
        auto it = textureDB.find (key);
        return it != textureDB.end() ? it->second : nullptr;
    }

    void setIndex (int i) { index = i; }

    bool isReady()
    {
        bool ready = true;

        if (base_color_tex)
        {
            for (auto img : base_color_tex->getImages())
            {
                if (!img->spec.ready)
                {
                    ready = false;
                    break;
                }
            }
        }

        if (metallic_roughness_tex)
        {
            for (auto img : metallic_roughness_tex->getImages())
            {
                if (!img->spec.ready)
                {
                    ready = false;
                    break;
                }
            }
        }

        if (normal_tex)
        {
            for (auto img : normal_tex->getImages())
            {
                if (!img->spec.ready)
                {
                    ready = false;
                    break;
                }
            }
        }

        return ready;
    }

    std::string name = "DEFAULT_MATERIAL_NAME";
    Vector4f base_color = {1.0f, 1.0f, 1.0f, 1.0f};
    float metallic = 0.0f;
    float roughness = 0.0f;
    int index = INVALID_INDEX;

    TextureHandle base_color_tex = nullptr;
    TextureHandle metallic_roughness_tex = nullptr;
    TextureHandle normal_tex = nullptr;

    Images getTextureImages()
    {
        Images images;

        if (base_color_tex)
            images.insert (std::end (images), std::begin (base_color_tex->getImages()), std::end (base_color_tex->getImages()));

        if (metallic_roughness_tex)
            images.insert (std::end (images), std::begin (metallic_roughness_tex->getImages()), std::end (metallic_roughness_tex->getImages()));

        if (normal_tex)
            images.insert (std::end (images), std::begin (normal_tex->getImages()), std::end (normal_tex->getImages()));

        return std::move (images);
    }

    TextureDB textureDB;
};

using Materials = std::vector<Material>;
using MaterialDB = std::unordered_map<MaterialID, MaterialHandle>;
