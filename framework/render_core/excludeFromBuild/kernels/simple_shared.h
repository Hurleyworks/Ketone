#include "common_shared.h"

using namespace Shared;

namespace SimpleShared
{
    enum RayType
    {
        RayType_Primary = 0,
        NumRayTypes
    };

    struct PipelineLaunchParameters
    {
        OptixTraversableHandle travHandle;
        int2 imageSize;
        optixu::BlockBuffer2D<float4, 1> resultBuffer;
        PerspectiveCamera camera;
        PickingData* pickData;
        float2 pickingFragment;
        int pickingEnabled;
    };
} // namespace SimpleShared
