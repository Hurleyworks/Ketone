#pragma once

#include <unordered_map>
#include <unordered_set>
#include <array>
#include <queue>
#include <stack>
#include <fstream>
#include <set>
#include <vector>
#include <sstream>
#include <iomanip> // byteStr()
#include <random>
#include <chrono>
#include <thread>
#include <ctime>
#include <string>
#include <iostream>
#include <stdexcept>
#include <assert.h>
#include <limits>
#include <algorithm>
#include <functional>
#include <stdint.h>
#include <any>

// Eigen
#include <EigenHeaders.h>

// juce
#include "juce_core/juce_core.h"

// copper spice signal/slots
#include <cs_signal.h>
#include <cs_slot.h>

// random
#include <random/include/random.hpp>
using RandoM = effolkronium::random_static;

// json
#include <json/single_include/nlohmann/json.hpp>
using nlohmann::json;

// g3log
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>

// openimageio
#include <OpenImageIO/thread.h>
#include <OpenImageIO/unordered_map_concurrent.h>
#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <OpenImageIO/imageio.h>

// tbb
#include <tbb/tbb/tbb.h>

// some useful tools and defines
#include "excludeFromBuild/basics/Defaults.h"
#include "excludeFromBuild/basics/Util.h"
#include "excludeFromBuild/tools/Tools.h"

namespace mace
{
#include "excludeFromBuild/basics/StringUtil.h"
}
