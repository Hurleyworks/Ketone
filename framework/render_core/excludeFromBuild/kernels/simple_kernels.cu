#pragma once

#include "simple_shared.h"

using namespace Shared;
using namespace SimpleShared;

RT_PIPELINE_LAUNCH_PARAMETERS PipelineLaunchParameters plp;

struct HitPointParameter
{
    float b1, b2;
    int32_t primIndex;

    CUDA_DEVICE_FUNCTION static HitPointParameter get()
    {
        HitPointParameter ret;
        float2 bc = optixGetTriangleBarycentrics();
        ret.b1 = bc.x;
        ret.b2 = bc.y;
        ret.primIndex = optixGetPrimitiveIndex();
        return ret;
    }
};

struct HitGroupSBTRecordData
{
    MaterialData matData;
    GeometryData geomData;

    CUDA_DEVICE_FUNCTION static const HitGroupSBTRecordData& get()
    {
        return *reinterpret_cast<HitGroupSBTRecordData*> (optixGetSbtDataPointer());
    }
};

#define PayloadSignature float3

CUDA_DEVICE_KERNEL void RT_RG_NAME (raygen)()
{
    uint2 launchIndex = make_uint2 (optixGetLaunchIndex().x, optixGetLaunchIndex().y);

    float x = static_cast<float> (launchIndex.x + 0.5f) / plp.imageSize.x;
    float y = static_cast<float> (plp.imageSize.y - launchIndex.y - 0.5f) / plp.imageSize.y;
    if (plp.pickingEnabled)
    {
        x = plp.pickingFragment.x / plp.imageSize.x;
        y = plp.pickingFragment.y / plp.imageSize.y;
    }
    plp.pickData->intanceID = ~0;
    plp.pickData->primitiveID = ~0;

    float vh = 2 * std::tan (plp.camera.fovY * 0.5f);
    float vw = plp.camera.aspect * vh;

    float3 origin = plp.camera.position;
    float3 direction = normalize (plp.camera.orientation * make_float3 (vw * (0.5f - x), vh * (y - 0.5f), 1));

    if (plp.pickingEnabled)
    {
        printf ("####xx Pick Data ####\n");
        printf ("Pick origin\n %f %f %f\n",
                origin.x, origin.y, origin.z);
        printf ("Pick direction\n %f %f %f\n",
                direction.x, direction.y, direction.z);
       
    }

    float3 color;
    optixu::trace<PayloadSignature> (
        plp.travHandle, origin, direction,
        0.0f, FLT_MAX, 0.0f, 0xFF, OPTIX_RAY_FLAG_NONE,
        RayType_Primary, NumRayTypes, RayType_Primary,
        color);

    plp.resultBuffer[launchIndex] = make_float4 (color, 1.0f);
}

CUDA_DEVICE_KERNEL void RT_MS_NAME (miss)()
{
    float3 color = make_float3 (0, 0, 0.1f);
    optixu::setPayloads<PayloadSignature> (&color);
}

CUDA_DEVICE_KERNEL void RT_CH_NAME (closesthit)()
{
    auto sbtr = HitGroupSBTRecordData::get();
    const GeometryData& geom = sbtr.geomData;
    const MaterialData& mat = sbtr.matData;

    auto hp = HitPointParameter::get();

    const Triangle& triangle = geom.triangleBuffer[hp.primIndex];
    const Vertex& v0 = geom.vertexBuffer[triangle.index0];
    const Vertex& v1 = geom.vertexBuffer[triangle.index1];
    const Vertex& v2 = geom.vertexBuffer[triangle.index2];

    if (plp.pickingEnabled)
    {
        PickingData* pickData = plp.pickData;
        pickData->primitiveID = hp.primIndex;
        pickData->intanceID = optixGetInstanceIndex();

        const float3 ray_orig = optixGetWorldRayOrigin();
        const float3 ray_dir = optixGetWorldRayDirection();
        const float ray_t = optixGetRayTmax();

        float3 spot = ray_orig + ray_t * ray_dir;
        pickData->hitPoint = spot;

        printf ("Hit point\n %f %f %f\n",
                spot.x, spot.y, spot.z);
        printf ("############################################\n");
        return;
    }

    float b0 = 1 - (hp.b1 + hp.b2);

    float3 sn = b0 * v0.normal + hp.b1 * v1.normal + hp.b2 * v2.normal;
    sn = normalize (optixTransformNormalFromObjectToWorldSpace (sn));

    float3 color = 0.5f * sn + make_float3 (0.5f);
    float3 albedo = mat.albedo;
    color.x *= albedo.x;
    color.y *= albedo.y;
    color.z *= albedo.z;

    optixu::setPayloads<PayloadSignature> (&color);
}
