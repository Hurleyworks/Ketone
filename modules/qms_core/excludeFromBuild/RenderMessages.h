#pragma once

using sabi::CameraHandle;
using sabi::PixelBufferHandle;

struct initRenderEngine
{
    initRenderEngine (const CameraHandle& camera) :
        camera (camera)
    {
    }
    QmsID id = QmsID::InitRenderEngine;
    CameraHandle camera = nullptr;
};

struct addNodeToRenderer
{
    addNodeToRenderer (const RenderableNode& node) :
        node (node)
    {
    }

    QmsID id = QmsID::AddNodeToRenderer;
    RenderableNode node;
};

struct renderNextFrame
{
    renderNextFrame (const CameraHandle& camera, uint32_t frameNumber = 0) :
        camera (camera),
        frameNumber (frameNumber)
    {
    }
    QmsID id = QmsID::RenderNextFrame;
    CameraHandle camera = nullptr;
    uint32_t frameNumber = 0;
};
