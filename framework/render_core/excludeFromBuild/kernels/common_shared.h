#pragma once

// from https://github.com/shocker-0x15/OptiX7_Utility

#include <optiX7Util/src/common.h>

namespace Shared
{
    struct PickingData
    {
        uint32_t intanceID;
        uint32_t primitiveID;
        float3 hitPoint;
    };

    struct Vertex
    {
        float3 position;
        float3 normal;
        float2 texCoord;
    };

    struct Triangle
    {
        uint32_t index0, index1, index2;
    };

    struct AABBb
    {
        float3 minP;
        float3 maxP;
    };

    struct SphereParameter
    {
        float3 center;
        float radius;
        float texCoordMultiplier;
    };

    struct PerspectiveCamera
    {
        float aspect;
        float fovY;
        float3 position;
        Matrix3x3 orientation;
    };

    class PCG32RNG
    {
        uint64_t state;

     public:
        CUDA_DEVICE_FUNCTION PCG32RNG() {}

        void setState (uint32_t _state) { state = _state; }

        CUDA_DEVICE_FUNCTION uint32_t operator()()
        {
            uint64_t oldstate = state;
            // Advance internal state
            state = oldstate * 6364136223846793005ULL + 1;
            // Calculate output function (XSH RR), uses old state for max ILP
            uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
            uint32_t rot = oldstate >> 59u;
            return (xorshifted >> rot) | (xorshifted << ((-(int32_t)rot) & 31));
        }

        CUDA_DEVICE_FUNCTION float getFloat0cTo1o()
        {
            uint32_t fractionBits = ((*this)() >> 9) | 0x3f800000;
            return *(float*)&fractionBits - 1.0f;
        }
    };

    struct GeometryData
    {
        const Vertex* vertexBuffer;
        const Triangle* triangleBuffer;
    };

    struct MaterialData
    {
        CUtexObject texture;
        float3 albedo;
        bool isEmitter;

        MaterialData() :
            texture (0),
            albedo (make_float3 (0.0f, 0.0f, 0.5f)),
            isEmitter (false) {}
    };

} // namespace Shared