#pragma once

static const char* RenderKeyTable[] =
    {
        "ResourceFolder",
        "CommonFolder",
        "RenderTime",
        "RenderState",
        "MotionBlur",
        "RenderType",
        "RenderOutput",
        "ShowPerformanceGraph,"
        "PtxFolder",
        "BackgroundColor",
        "Invalid"};

struct RenderKey
{
    enum ERenderKey
    {
        ResourceFolder,
        CommonFolder,
        RenderTime,
        RenderState,
        MotionBlur,
        RenderType,
        RenderOutput,
        ShowPerformanceGraph,
        PtxFolder,
        BackgroundColor,
        Count,
        Invalid = Count
    };

    union
    {
        ERenderKey name;
        unsigned int value;
    };

    RenderKey (ERenderKey name) :
        name (name) {}
    RenderKey (unsigned int value) :
        value (value) {}
    RenderKey() :
        value (Invalid) {}
    operator ERenderKey() const { return name; }
    const char* ToString() const { return RenderKeyTable[value]; }
    static RenderKey FromString (const char* str) { return mace::TableLookup (str, RenderKeyTable, Count); }
};

using RenderProperties = AnyValue<RenderKey>;
using RenderPropsRef = std::shared_ptr<RenderProperties>;
const RenderMode DEFAULT_RENDER_TYPE = RenderMode (RenderMode::PathTracer);
const RenderOutput DEFAULT_RENDER_OUTPUT = RenderOutput (RenderOutput::ColorDenoised);
const bool DEFAULT_MOTION_BLUR = false;