
project "oiio"

if _ACTION == "vs2019" then
	location ("../builds/VisualStudio2019/projects")
end
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    flags { "MultiProcessorCompile" }
	defines {"OIIO_STATIC_DEFINE", "EMBED_PLUGINS"}
	targetdir ("../builds/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../builds/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"../precompiled/include/OpenEXR",
		"../precompiled/include",
		"../precompiled/boost",
		"..",
		"source",
        "source/*",
    }

	files
	{
		"source/**.h", 
		"source/**.cpp", 
		"source/**.c", 
    }
	
	-- exclude the stupid test files in OIIO
	excludes {
		"source/libutil/*_test*.cpp",
		"source/libOpenImageIO/*_test*.cpp",
	}
	
	filter "system:windows"
        staticruntime "On"
        systemversion "latest"
        disablewarnings { "4244", "4267", "4996", "4305", "4018", "4334", "4312", "4311", "4251", "4275", "4244" }
		defines { "_CRT_SECURE_NO_WARNINGS"}
		characterset "MBCS"

    filter "configurations:Debug"
        symbols "On"
                
    filter "configurations:Release"
        optimize "On"
	