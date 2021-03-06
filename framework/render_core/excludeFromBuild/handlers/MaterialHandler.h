// This header file was auto-generated by ClassMate++
// Created: 8 Sep 2020 8:44:40 pm
// Copyright (c) 2020, HurleyWorks

#pragma once

using sabi::RenderableNode;
using sabi::Surfaces;

using MaterialHandlerRef = std::shared_ptr<class MaterialHandler>;

class MaterialHandler
{
 public:
    static MaterialHandlerRef create (OptiXStateRef state) { return std::make_shared<MaterialHandler> (state); }

    using OptixMaterials = std::vector<optixu::Material>;
    using MaterialSets = std::vector<OptixMaterials>;
    using MaterialMap = std::unordered_map<RenderableNode, MaterialSets>;
    using MaterialIndexBuffer = cudau::TypedBuffer<uint32_t>;
    using MaterialIndexBufferRef = std::shared_ptr<MaterialIndexBuffer>;
    using MaterialIndexBuffers = std::vector<MaterialIndexBufferRef>;

 public:
    MaterialHandler (OptiXStateRef state);
    ~MaterialHandler();

    uint32_t addRenderableMaterials (RenderableNode& node, GeometryInstanceRef& geomInst, PipelineHandlerRef& pipeline);

    const MaterialSets& getMaterialSets (RenderableNode& node)
    {
        auto it = materialMap.find (node);
        return it == materialMap.end() ? emptySet : it->second;
    }

 private:
    OptiXStateRef state = nullptr;
    MaterialMap materialMap;
    MaterialSets emptySet;

    MaterialIndexBuffers matIndxBuffers;
    std::vector<uint32_t> matIndices;

    void createOptixMaterials (RenderableNode& node, PipelineHandlerRef& pipeline);
    void addRandomMaterialSet (OptixMaterials& matSet, const Surfaces& surfs, PipelineHandlerRef& pipeline);

}; // end class MaterialHandler
