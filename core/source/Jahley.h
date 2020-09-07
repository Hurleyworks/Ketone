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

inline String getRepositoryPath(const String& appName)
{
    File exe = File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile);
    String exefullPath = exe.getFullPathName();

    String pathToRepro = exefullPath.upToFirstOccurrenceOf(REPOSITORY_NAME, true, false);
    return pathToRepro;
}
