// This header file was auto-generated by ClassMate++
// Created: 9 Aug 2020 3:08:12 pm
// Copyright (c) 2020, HurleyWorks

#pragma once

using MoojiRendererRef = std::shared_ptr<class IRenderer>;
using sabi::InputEvent;

class IRenderer
{
 public:
    const uint32_t DEFAULT_PASSES = 5;

 public:
    virtual ~IRenderer()
    {
        rngBuffer.finalize();
    }

    virtual void initialize (MoojiStateRef& state, CameraHandle& camera)
    {
        this->state = state;

        gpuTimer[0].initialize (state->cuContext);
        gpuTimer[1].initialize (state->cuContext);
    }

    virtual void generatePickRay (float x, float y) {}
    virtual void updateCamera (CameraHandle& camera) = 0;
    virtual void onResize (CameraHandle& camera) = 0;
    virtual float render (CameraHandle& camera, InputEvent& input, uint32_t frameNumber, bool reset = false) = 0;
    virtual void initializeLaunchParameters (CameraHandle& camera) = 0;

 protected:
    IRenderer (const PropertyService& properties) :
        properties (properties)
    {
    }
    PropertyService properties;
    MoojiStateRef state = nullptr;
    GPUTimer gpuTimer[2];

    int32_t renderWidth = DEFAULT_DESKTOP_WINDOW_WIDTH;
    int32_t renderHeight = DEFAULT_DESKTOP_WINDOW_HEIGHT;

    uint64_t frameIndex = 0;
    uint32_t passes = DEFAULT_PASSES;

    optixu::HostBlockBuffer2D<Shared::PCG32RNG, 1> rngBuffer;
    void rebuildRNG()
    {
        rngBuffer.initialize (state->engine.cuCtx(), cudau::BufferType::Device, renderWidth, renderHeight);
        {
            std::mt19937_64 rng (591842031321323413);

            rngBuffer.map();
            for (int y = 0; y < renderHeight; ++y)
                for (int x = 0; x < renderWidth; ++x)
                    rngBuffer (x, y).setState (rng());
            rngBuffer.unmap();
        };
    }

}; // end class IRenderer
