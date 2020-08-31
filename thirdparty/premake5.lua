
-- https://github.com/JohannesMP/Premake-for-Beginners

workspace "ThirdParty"
	architecture "x64"
	location ("builds")
	
if _ACTION == "vs2017" then
   location ("builds/VisualStudio2017")
end
if _ACTION == "vs2019" then
   location ("builds/VisualStudio2019")
end
if _ACTION == "vs2015" then
   location ("builds/VisualStudio2015")
end
	
	configurations 
	{ 
		"Debug", 
        "Release",
    }
	
	filter "configurations:Debug"    defines { "DEBUG" }  symbols  "On"
    filter "configurations:Release"  defines { "NDEBUG" } optimize "On"
    
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

     
-- create projects 
include "nanogui"
include "nanogui/ext/glfw"
include "stb"
include "g3log"
include "cs_signal"
include "optiX7Util"
include "JUCE"
include "oiio"
include "fast_obj"
	