//
// Note:	this file is to be included in client applications ONLY
//			NEVER include this file anywhere in the engine codebase
//
#pragma once

#include "jahley/AppConfig.h"
#include "jahley/core/App.h"
#include "jahley/EntryPoint.h"

using juce::File;
using juce::String;
const String REPOSITORY_NAME = "Ketone";

inline String getResourcePath (const String& appName)
{
    File exe = File::getSpecialLocation (File::SpecialLocationType::currentExecutableFile);
    String exefullPath = exe.getFullPathName();

    String pathToRepro = exefullPath.upToFirstOccurrenceOf (REPOSITORY_NAME, true, false);
    return pathToRepro + "/resources/" + appName;
}

inline String getPtxFolder (const String& appName)
{
    // by default compiled PTX files are put in the intermeditate object folder
    File exe = File::getSpecialLocation (File::SpecialLocationType::currentExecutableFile);
    String exefullPath = exe.getFullPathName();

    String pathToPtx = exefullPath.upToFirstOccurrenceOf ("builds", true, false);
    pathToPtx = pathToPtx + "/bin-int/";

    String configuration;
#ifdef NDEBUG
    configuration = "Release-windows-x86_64/";
#else
    configuration = "Debug-windows-x86_64/";
#endif

    pathToPtx = pathToPtx + configuration + appName;

    return pathToPtx;
}