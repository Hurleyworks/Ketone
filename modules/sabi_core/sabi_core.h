#pragma once

#include "../wabi_core/wabi_core.h"

namespace sabi
{
#include "excludeFromBuild/PixelBuffer.h"

// mesh
#include "excludeFromBuild/mesh/VertexMap.h"
#include "excludeFromBuild/mesh/MeshOptions.h"
#include "excludeFromBuild/mesh/TextureHandler.h"
#include "excludeFromBuild/mesh/Material.h"
#include "excludeFromBuild/mesh/Surface.h"
#include "excludeFromBuild/mesh/MeshState.h"
#include "excludeFromBuild/mesh/MeshBuffers.h"

// scene
#include "excludeFromBuild/scene/ActiveCam.h"
#include "excludeFromBuild/scene/SpaceTime.h"
#include "excludeFromBuild/scene/RenderableData.h"
#include "excludeFromBuild/scene/RenderableState.h"
#include "excludeFromBuild/scene/RenderableDesc.h"
#include "excludeFromBuild/scene/Renderable.h"
#include "excludeFromBuild/scene/WorldItem.h"
#include "excludeFromBuild/scene/WorldComposite.h"
#include "excludeFromBuild/scene/InputEvent.h"

// tools
#include "excludeFromBuild/tools/MeshOps.h"
#include "excludeFromBuild/tools/LoadStrategy.h"
#include "excludeFromBuild/tools/NormalizedClump.h"

} // namespace sabi

// keep properties out of any namespace for now
#include "excludeFromBuild/properties/WorldProps.h"
#include "excludeFromBuild/properties/RenderProps.h"
#include "excludeFromBuild/properties/IOProps.h"
#include "excludeFromBuild/properties/PropertiesServices.h"