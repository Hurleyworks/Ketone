

project "JUCE"
  if _ACTION == "vs2019" then
	location ("../builds/VisualStudio2019/projects")
   end
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
    flags { "MultiProcessorCompile" }
	characterset "MBCS"
	
	targetdir ("../builds/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../builds/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{ 
		"modules/juce_core/**.h", 
		"modules/juce_core/juce_core.cpp", 
    }

    includedirs
	{
		"..",
		"modules"
    }
    
	filter "system:windows"
        staticruntime "On"
        systemversion "latest"
       
		defines 
		{ 
			"_CRT_SECURE_NO_WARNINGS"
		}

    filter "configurations:Debug"
        symbols "On"
                
    filter "configurations:Release"
        optimize "On"






