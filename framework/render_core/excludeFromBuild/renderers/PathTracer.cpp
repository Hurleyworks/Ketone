#include "PathTracer.h"
// This source file was auto-generated by ClassMate++
// Created: 9 Aug 2020 3:08:12 pm
// Copyright (c) 2020, HurleyWorks

using sabi::PRenderableState;

// ctor
PathTracer::PathTracer (const PropertyService& properties) :
    IRenderer (properties)
{
}

// dtor
PathTracer::~PathTracer()
{
    try
    {
        CUDADRV_CHECK (cuStreamSynchronize (state->engine.stream()));
        CUDADRV_CHECK (cuMemFree (plpOnDevice));
    }
    catch (std::exception& e)
    {
        LOG (CRITICAL) << e.what();
    }
    catch (...)
    {
        LOG (CRITICAL) << "Caught unknown exception!";
    }
}

float PathTracer::render (CameraHandle& camera, InputEvent& input, uint32_t frameNumber, bool reset)
{
    if (reset) frameIndex = 0;


    plp.travHandle = state->groups.size() ? state->groups[0]->optixIAS.getHandle() : 0;
    plp.lightGeomInstIndex = state->areaLightIndex;

    CUstream& curCuStream = state->engine.stream();
  
    if (plp.pickingEnabled)
    {
        pick(input, curCuStream);
 
    }
      

   

    gpuTimer.render.start (curCuStream);
    constexpr uint32_t numSamples = 1;
    for (int pass = 0; pass < numSamples; ++pass)
    {
        plp.numAccumFrames = frameIndex;
        CUDADRV_CHECK (cuMemcpyHtoDAsync (plpOnDevice, &plp, sizeof (plp), curCuStream));
        state->engine.pl().launch (curCuStream, plpOnDevice, renderWidth, renderHeight, 1);
    }
    CUDADRV_CHECK(cuStreamSynchronize(curCuStream));
    gpuTimer.render.stop (curCuStream);
    denoiser.update (curCuStream, renderWidth, renderHeight);

    // render time is in ms
    float renderTime = gpuTimer.render.report();

    RenderOutput outputType = properties.renderProps->getVal<RenderOutput> (RenderKey::RenderOutput);
    PixelBuffer& pixels = camera->getPixelBuffer();

    denoiser.getRender (pixels, outputType);

    ++frameIndex;

    return renderTime;
}

void PathTracer::updateCamera (CameraHandle& camera)
{
    camera->getViewMatrix();
    camera->getProjectionMatrix();

    const Vector3f& eye = camera->getEyePoint();
    const Vector3f& forward = camera->getFoward();
    const Vector3f& right = camera->getRight();
    const Vector3f& up = camera->getUp();

    float3 camRight, camUp, camForward, camEye;

    camUp = make_float3 (up.x(), up.y(), up.z());
    camRight = make_float3 (right.x(), right.y(), right.z());
    camForward = make_float3 (forward.x(), forward.y(), forward.z());
    camEye = make_float3 (eye.x(), eye.y(), eye.z());

    plp.camera.aspect = (float)renderWidth / renderHeight;
    plp.camera.fovY = camera->getFOV();
    plp.camera.position = camEye;

    // FIXME why do we have to negate the right vector to make this work????
    plp.camera.orientation = Matrix3x3 (camRight  * -1.0f, camUp, camForward);
}

void PathTracer::pick (InputEvent& input, CUstream& cuStream)
{
    CUDADRV_CHECK (cuMemcpyHtoDAsync (plpOnDevice, &plp, sizeof (plp), cuStream));
    state->engine.pl().launch (cuStream, plpOnDevice, 1, 1, 1);
    CUDADRV_CHECK (cuStreamSynchronize (cuStream));

    state->pickDataBuffer.map();

    Shared::PickingData* pickData = state->pickDataBuffer.getMappedPointer();
    if (!pickData) return;

    // hit no geometry and we're done
    if (pickData->intanceID == ~0)
    {
        resetPicking();
        state->pickDataBuffer.unmap();
        return;
    }
    if (pickData->intanceID >= state->groups[0]->insts.size())
    {
        LOG (CRITICAL) << "Out of bounds instanceID";
        resetPicking();
        state->pickDataBuffer.unmap();
        return;
    }

    InstanceRef inst = state->groups[0]->insts[pickData->intanceID];
    if (!inst)
    {
        resetPicking();
        state->pickDataBuffer.unmap();
        return;
    }

    LOG (DBUG) << pickData->intanceID << ",  " << pickData->primitiveID << "::" << inst->node->getName();
    //LOG(DBUG) <<  inst->node->getState().toString();

    state->pickDataBuffer.unmap();

    resetPicking();



    if (inst->node)
    {
        // toggle selection state
        if (inst->node->getState().isSelected())
        {
            // lower the selected flag
            inst->node->getState().state ^= PRenderableState::Selected;

            // remove it from the selected set
            auto it = state->selected.find (inst);
            if (it != state->selected.end())
            {
                state->selected.erase (it);

                // add it to the deselected set
                state->deselected.insert (std::make_pair (inst, inst->node));
            }
        }
        else
        {
            // raise the selected flag
            inst->node->getState().state |= PRenderableState::Selected;

            // add it to the selected set
            state->selected.insert (std::make_pair (inst, inst->node));
        }

        input.setPickedNode (inst->node);
    }
}

void PathTracer::resetPicking()
{
    plp.pickingEnabled = 0;
    plp.pickingFragment = make_float2(-1.0f, -1.0f);
}

void PathTracer::initializeLaunchParameters (CameraHandle& camera)
{
    rebuildRNG();

    plp.travHandle = state->groups.size() ? state->groups[0]->optixIAS.getHandle() : 0;
    plp.geomInstData = state->geometryDataBuffer.getDevicePointer();
    plp.pickData = state->pickDataBuffer.getDevicePointer();
    plp.materialData = state->materialDataBuffer.getDevicePointer();
    plp.imageSize = int2 (renderWidth, renderHeight);
    plp.rngBuffer = rngBuffer.getBlockBuffer2D();
    plp.colorAccumBuffer = denoiser.getColorAccumBuffer();
    plp.albedoAccumBuffer = denoiser.getAlbedoAccumBuffer();
    plp.normalAccumBuffer = denoiser.getNormalAccumBuffer();
    plp.lightGeomInstIndex = 0; // fixme
    plp.pickingEnabled = 0;
    plp.pickingFragment = make_float2 (-1.0f, -1.0f);

    state->engine.pl().setScene (state->scene);
    state->engine.pl().setHitGroupShaderBindingTable (&state->hitGroupSBT[0]);

    updateCamera (camera);

    CUDADRV_CHECK (cuMemAlloc (&plpOnDevice, sizeof (plp)));
}

void PathTracer::resize (CameraHandle& camera)
{
    camera->getPixelBuffer().resize (camera->getScreenWidth(), camera->getScreenHeight());

    renderWidth = static_cast<uint32_t> (camera->getScreenWidth());
    renderHeight = static_cast<uint32_t> (camera->getScreenHeight());

    rngBuffer.finalize();
    rebuildRNG();

    //accumBuffer.finalize();
    //accumBuffer.initialize (state->cuContext, cudau::BufferType::Device, renderWidth, renderHeight);

    //plp.imageSize = int2 (renderWidth, renderHeight);
    //plp.accumBuffer = accumBuffer.getBlockBuffer2D();
    //plp.rngBuffer = rngBuffer.getBlockBuffer2D();
    //plp.timeBegin = 0.0f;
    //plp.timeEnd = 1.0f;
    //plp.numAccumFrames = 0;

    //updateCamera (camera);

    //// front end can safely render now
    //camera->getPixelBuffer().spec.dirty = false;
}
