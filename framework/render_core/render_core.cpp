
#include "berserkpch.h"
#include "render_core.h"

#include "excludeFromBuild/ActiveRender.cpp"
#include "excludeFromBuild/RenderCore.cpp"
#include "excludeFromBuild/OptiXState.cpp"

// handlers
#include "excludeFromBuild/handlers/NoiseHandler.cpp"
#include "excludeFromBuild/handlers/MeshHandler.cpp"
#include "excludeFromBuild/handlers/MotionHandler.cpp"
#include "excludeFromBuild/handlers/MaterialHandler.cpp"
#include "excludeFromBuild/handlers/PipelineHandler.cpp"
#include "excludeFromBuild/handlers/SceneHandler.cpp"
#include "excludeFromBuild/handlers/PickHandler.cpp"

// renderers
#include "excludeFromBuild/renderers/SimpleRenderer.cpp"

// nvcc
#include "excludeFromBuild/nvcc/CudaCompiler.cpp"