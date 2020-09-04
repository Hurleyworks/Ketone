#pragma once

#include "ActiveFramework.h"

using Eigen::Vector2f;
using Eigen::Vector3f;
using juce::String;
using sabi::CameraHandle;
using sabi::InputEvent;
using sabi::PixelBufferHandle;
using sabi::RenderableDesc;

class Model : public CsSignal::SlotBase, public CsSignal::SignalBase
{
    //https://www.bogotobogo.com/DesignPatterns/mvc_model_view_controller_pattern.php
    // Manages the app data and state
    // Decoupled from UI or presentation
    // Same model should be reusable, unchanged in different interfaces

 public:
    Model() = default;
    ~Model() = default;

    ActiveFramework* m() { return &framework; }
    bool shutdown() { return framework.dreamer.terminate(); }

    void initializeFramework (PropertyService* properties)
    {
        jassert (properties);
        framework.init (*properties);
        this->properties = properties;
    }

    void initializeRenderEngine (const CameraHandle& camera)
    {
        framework.render.getMessenger().send (QMS::initRenderEngine (camera));
    }

    void renderNextFrame (const CameraHandle& camera)
    {
        framework.render.getMessenger().send (QMS::renderNextFrame (camera, hackFrameNumber++));
    }

    void onInput (const InputEvent& input)
    {
        framework.render.getMessenger().send (QMS::onInput (input));
    }

    void createGroundPlane (const Vector2f& size = Vector2f (100.0f, 100.0f))
    {
        framework.geometry.getMessenger().send (QMS::makeGroundPlane (size));
    }

    void loadModelFromIcon (const std::string& iconPath);

    void onDeselectAll()
    {
        framework.world.getMessenger().send (QMS::deselectAll());
    }

    void createInstanceClump (uint32_t count)
    {
        framework.world.getMessenger().send (QMS::createInstanceClump (count));
    }

    void onClearScene()
    {
    }

    void onDrop (const std::vector<std::string>& fileList);

 private:
    ActiveFramework framework;
    PropertyService* properties = nullptr;
    uint32_t frameNumber = 0;
    uint32_t hackFrameNumber = 0;
    bool sceneSavingOn = false;
};
