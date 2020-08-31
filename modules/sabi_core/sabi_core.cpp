// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.

#include "berserkpch.h"
#include "sabi_core.h"

namespace sabi
{
// scene
#include "excludeFromBuild/scene/InputEvent.cpp"
#include "excludeFromBuild/scene/ActiveCam.cpp"
#include "excludeFromBuild/scene/Spacetime.cpp"
#include "excludeFromBuild/scene/WorldComposite.cpp"
#include "excludeFromBuild/scene/WorldItem.cpp"

// tools
#include "excludeFromBuild/tools/MeshOps.cpp"
#include "excludeFromBuild/tools/NormalizedClump.cpp"

} // namespace sabi
