// This header file was auto-generated by ClassMate++
// Created: 29 Dec 2018 1:52:03 pm
// Copyright (c) 2018, HurleyWorks

#pragma once

using sabi::LoadStrategyHandle;
using sabi::RenderableNode;

class WorldOps
{
    // The purpose of this class is to handle any operations that
    // require iterating through the scene graph

 public:
    WorldOps (RenderableNode world);
    ~WorldOps();

    void init (MessageService messengers, PropertyService properties)
    {
        this->messengers = messengers;
        this->properties = properties;
    }

    void createInstanceClump (int instanceCount);

 private:
    RenderableNode world = nullptr;
    MessageService messengers;
    PropertyService properties;
    LoadStrategyHandle loadStrategy = nullptr;

}; // end class WorldOps
