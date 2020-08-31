local ROOT = "../../"

project  "ActiveKetone"
	if _ACTION == "vs2019" then
		cppdialect "C++17"
		location (ROOT .. "builds/VisualStudio2019/projects")
    end
	if _ACTION == "vs2017" then
		location (ROOT .. "builds/VisualStudio2017/projects")
    end
	if _ACTION == "vs2015" then
		location (ROOT .. "builds/VisualStudio2015/projects")
    end
	
	kind "ConsoleApp"
	
	local RESOURCES_ROOT = "../../resources/Dreamer/"
	local FRAMEWORK_ROOT = "../../framework/"
	
	local ACTIVE_RENDER = FRAMEWORK_ROOT .. "render_core/";
	local ACTIVE_GEO = FRAMEWORK_ROOT .. "geo_core/";
	local ACTIVE_IO = FRAMEWORK_ROOT .. "io_core/";
	local ACTIVE_WORLD = FRAMEWORK_ROOT .. "world_core/";
	
	local SOURCE_DIR = "source/*"
	
    files
    { 
      SOURCE_DIR .. "**.h", 
      SOURCE_DIR .. "**.hpp", 
      SOURCE_DIR .. "**.c",
      SOURCE_DIR .. "**.cpp",
	  
	  -- add framework modules for this project
	  ACTIVE_RENDER ..  "**.h", 
	  ACTIVE_RENDER .. "**.cpp",
	  ACTIVE_RENDER .. "**.cu",
	  ACTIVE_GEO ..  "**.h", 
	  ACTIVE_GEO .. "**.cpp",
	  ACTIVE_IO ..  "**.h", 
	  ACTIVE_IO .. "**.cpp",
	  ACTIVE_WORLD ..  "**.h", 
	  ACTIVE_WORLD .. "**.cpp",
    }
	
	
	includedirs
	{
		FRAMEWORK_ROOT,
	}
	
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		defines {"_CRT_SECURE_NO_WARNINGS", "__WINDOWS_WASAPI__"}
		disablewarnings { "5030" , "4305", "4316", "4267"}
		vpaths 
		{
		  ["Header Files/*"] = { 
			SOURCE_DIR .. "**.h", 
			SOURCE_DIR .. "**.hxx", 
			SOURCE_DIR .. "**.hpp",
		  },
		  ["Source Files/*"] = { 
			SOURCE_DIR .. "**.c", 
			SOURCE_DIR .. "**.cxx", 
			SOURCE_DIR .. "**.cpp",
		  },
		}
		
	filter {} -- clear filter!
	filter { "files:../../framework/**/excludeFromBuild/**.cpp"}
	flags {"ExcludeFromBuild"}
	filter {} -- clear filter!
	
	
-- add settings common to all project
dofile("../../premake/common/common.lua")


	
