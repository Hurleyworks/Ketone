
// This source file was auto-generated by ClassMate++
// Created: 9 Sep 2020 6:36:59 am
// Copyright (c) 2020, HurleyWorks


using sabi::PixelBuffer;

// ctor
SimpleRenderer::SimpleRenderer (OptiXStateRef state) :
    IRenderer (state),
    picker(state)
{
    gpuTimer.initialize (state->cuContext);
}

// dtor
SimpleRenderer::~SimpleRenderer()
{
    accumBuffer.finalize();
    gpuTimer.finalize();

    try
    {
        CUDADRV_CHECK (cuStreamSynchronize (state->cuStream));
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

void SimpleRenderer::initialize (CameraHandle& camera)
{
    accumBuffer.initialize (state->cuContext, cudau::BufferType::Device, renderWidth, renderHeight);

    plp.travHandle = 0;
    plp.imageSize.x = renderWidth;
    plp.imageSize.y = renderHeight;
    plp.pickData = picker.getDevicePtr();
    plp.pickingEnabled = 0;
    plp.pickingFragment = make_float2 (-1.0f);
    plp.resultBuffer = accumBuffer.getBlockBuffer2D();
    updateCamera (camera);

    CUDADRV_CHECK (cuMemAlloc (&plpOnDevice, sizeof (plp)));
}

float SimpleRenderer::render (CameraHandle& camera, PipelineHandlerRef& pipeline)
{
    plp.travHandle = state->groups.size() ? state->groups[0]->optixIAS.getHandle() : 0;

    gpuTimer.render.start (state->cuStream);

    CUDADRV_CHECK (cuMemcpyHtoDAsync (plpOnDevice, &plp, sizeof (plp), state->cuStream));
    pipeline->pl().launch (state->cuStream, plpOnDevice, renderWidth, renderHeight, 1);
    CUDADRV_CHECK (cuStreamSynchronize (state->cuStream));

    gpuTimer.render.stop (state->cuStream);

    accumBuffer.map();

    PixelBuffer& pixels = camera->getPixelBuffer();
    uint32_t i = 0;
    for (int y = 0; y < renderHeight; ++y)
    {
        for (int x = 0; x < renderWidth; ++x)
        {
            float4& color = accumBuffer (x, y);
            /* color.x = sRGB_gamma_s (1 - std::exp (-color.x));
            color.y = sRGB_gamma_s (1 - std::exp (-color.y));
            color.z = sRGB_gamma_s (1 - std::exp (-color.z));*/
            pixels.floatPixels.col (i++) = Eigen::Vector4f (color.x, color.y, color.z, color.w);
        }
    }
    accumBuffer.unmap();

    // render time is in ms
    return gpuTimer.render.report();
}

void SimpleRenderer::updateCamera (CameraHandle& camera)
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

    plp.camera.orientation = Matrix3x3 (camRight * -1.0f, camUp, camForward);
}

void SimpleRenderer::onResize (CameraHandle& camera)
{
    camera->getPixelBuffer().resize (camera->getScreenWidth(), camera->getScreenHeight());

    renderWidth = static_cast<uint32_t> (camera->getScreenWidth());
    renderHeight = static_cast<uint32_t> (camera->getScreenHeight());

    accumBuffer.finalize();
    accumBuffer.initialize (state->cuContext, cudau::BufferType::Device, renderWidth, renderHeight);
    plp.resultBuffer = accumBuffer.getBlockBuffer2D();
 
    plp.imageSize.x = renderWidth;
    plp.imageSize.y = renderHeight;
    plp.camera.aspect = (float)renderWidth / renderHeight;
}
