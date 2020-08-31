#pragma once

// from https://github.com/shocker-0x15/OptiX7_Utility

#include "common_shared.h"

using namespace Shared;

namespace PathTracerShared
{
    static constexpr float Pi = 3.14159265358979323846f;

    enum RayType
    {
        RayType_Search = 0,
        RayType_Visibility,
        NumRayTypes
    };

    struct PipelineLaunchParameters
    {
        OptixTraversableHandle travHandle;
        const MaterialData* materialData;
        const GeometryData* geomInstData;
        PickingData* pickData;
        int2 imageSize;
        float2 pickingFragment;
        uint32_t numAccumFrames;
        int pickingEnabled;

        optixu::BlockBuffer2D<PCG32RNG, 1> rngBuffer;
        optixu::NativeBlockBuffer2D<float4> colorAccumBuffer;
        optixu::NativeBlockBuffer2D<float4> albedoAccumBuffer;
        optixu::NativeBlockBuffer2D<float4> normalAccumBuffer;
        PerspectiveCamera camera;
        uint32_t lightGeomInstIndex;
    };

} // namespace PathTracerShared
