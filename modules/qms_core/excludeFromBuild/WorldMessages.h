#pragma once

using sabi::RenderableData;
using sabi::RenderableDesc;
using sabi::RenderableNode;
using sabi::RenderableWeakRef;

struct deselectAll
{
    QmsID id = QmsID::DeselectAll;
};

struct createNewRenderable
{
    createNewRenderable (const RenderableData& data) :
        data (data)
    {
    }

    QmsID id = QmsID::TopPriority;
    QmsID cmdID = QmsID::CreateNewRenderable;

    RenderableData data;
};

struct createInstanceClump
{
    createInstanceClump (uint32_t count) : 
        count(count)
    {
    }

    QmsID id = QmsID::CreateInstanceClump;
    uint32_t count = 0;
};
