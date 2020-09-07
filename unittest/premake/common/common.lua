
local REPO_ROOT = "../../../"
local ROOT = "../../"
	language "C++"

	defines{
		"NANOGUI_GLAD", "JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED",
		"_USE_MATH_DEFINES", "_ENABLE_EXTENDED_ALIGNED_STORAGE", "__TBB_NO_IMPLICIT_LINKAGE",
	    "OIIO_STATIC_DEFINE", "_NEWTON_STATIC_LIB", "_CUSTOM_JOINTS_STATIC_LIB", "_DVEHICLE_STATIC_LIB",
	}

	flags { "MultiProcessorCompile", "NoMinimalRebuild" }
	
	local CORE_DIR = REPO_ROOT .. "core/source/"
	local JAHLEY_DIR = REPO_ROOT .. "core/source/jahley/"
	local THIRD_PARTY_DIR = REPO_ROOT .. "thirdparty/"
	local MODULE_DIR = REPO_ROOT .. "modules/"
	local FRAMEWORK_ROOT = REPO_ROOT .. "framework/"
	
	local CUDA_INCLUDE_DIR = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.0/include"
	local CUDA_EXTRA_DIR = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.0/extras/cupti/include"
	local CUDA_LIB_DIR =  "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.0/lib/x64"
	local OPTIX_ROOT = "C:/ProgramData/NVIDIA Corporation"
	local OPTIX7_INCLUDE_DIR = OPTIX_ROOT .. "/OptiX SDK 7.1.0/include"
	local CUDA_DNN_LIB_DIR = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/cudaDnn/lib/x64"
	
	includedirs
	{
		CORE_DIR,
		JAHLEY_DIR,
		MODULE_DIR,
		CUDA_INCLUDE_DIR,
		CUDA_EXTRA_DIR,
		OPTIX7_INCLUDE_DIR,
		THIRD_PARTY_DIR,
	    THIRD_PARTY_DIR .. "eigenMath",
		THIRD_PARTY_DIR .. "nanogui/ext/glfw/include",
		THIRD_PARTY_DIR .. "nanogui/include",
		THIRD_PARTY_DIR .. "nanogui/ext/glad/include",
		THIRD_PARTY_DIR .. "nanogui/ext/nanovg/src",
		THIRD_PARTY_DIR .. "g3log/src",
		THIRD_PARTY_DIR .. "doctest",
		THIRD_PARTY_DIR .. "stb/include",
		THIRD_PARTY_DIR .. "cs_signal/source",
		THIRD_PARTY_DIR .. "fast_obj/source",
		THIRD_PARTY_DIR .. "JUCE/modules",
		THIRD_PARTY_DIR .. "precompiled/include",
		THIRD_PARTY_DIR .. "precompiled/include/tbb",
		THIRD_PARTY_DIR .. "oiio/source/include",
		THIRD_PARTY_DIR .. "json/single_include/nlohmann",
		--THIRD_PARTY_DIR .. "newton-dynamics",
		THIRD_PARTY_DIR .. "newton4/sdk/dCore",
		THIRD_PARTY_DIR .. "newton4/sdk/ntNewton",
	}
	
	targetdir (ROOT .. "builds/bin/" .. outputdir .. "/%{prj.name}")
	objdir (ROOT .. "builds/bin-int/" .. outputdir .. "/%{prj.name}")
	
	filter { "system:windows"}
		disablewarnings { 
			"5030", "4244", "4267", "4667", "4018", "4101", "4305", "4316", "4146",
		} 
		characterset ("MBCS")
		buildoptions { "/Zm250", "/bigobj",}
		defines 
		{ 
			"WIN32",
			--https://github.com/KjellKod/g3log/issues/337
			"_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING",
		}
		
	filter "configurations:Debug"
		links 
		{ 
			"CoreLibrary",
			"Nanogui",
			"GLFW",
			"CoreLibrary",
			"g3log",
			"optiX7Util",
			"advapi32",
			"IPHLPAPI",
			"cs_signal",
			"fast_obj",
			"GLTFSDK",
			"JUCE",
			"tbb_static",
			"Imath-2_5_d",
			"boost_thread-vc140-mt-gd",
			"boost_filesystem-vc140-mt-gd",
			"boost_system-vc140-mt-gd",
			"Half-2_5_d",
			"Iex-2_5_d",
			"IexMath-2_5_d",
			"IlmImf-2_5_d",
			"IlmImfUtil-2_5_d",
			"IlmThread-2_5_d",
			"Imath-2_5_d",
			"jpegd",
			"tiffd",
			"zlibd",
			"lzmad",
			"oiio",
			"Field3D",
			"libpng16d",
			"NewtonSDK4",
			--cuda
			"cudart_static",
			"cuda",
			"nvrtc",
			"cudnn",
			"cublas",
			"curand",
			"cusolver",
			"cudart",
		}
		defines { "DEBUG", "USE_DEBUG_EXCEPTIONS" }
		symbols "On"
		libdirs { THIRD_PARTY_DIR .. "builds/bin/" .. outputdir .. "/**",
				  THIRD_PARTY_DIR .. "precompiled/bin/" .. outputdir .. "/**",
				  REPO_ROOT .. "builds/bin/" .. outputdir .. "/**",
				  CUDA_LIB_DIR, CUDA_DNN_LIB_DIR,
		}
		
	filter "configurations:Release"
		links 
		{ 
			"CoreLibrary",
			"Nanogui",
			"GLFW",
			"CoreLibrary",
			"g3log",
			"optiX7Util",
			"advapi32",
			"IPHLPAPI",
			"cs_signal",
			"JUCE",
			"tbb_static",
			"fast_obj",
			"GLTFSDK",
			"Imath-2_5",
			"boost_thread-vc140-mt",
			"boost_filesystem-vc140-mt",
			"boost_system-vc140-mt",
			"Half-2_5",
			"Iex-2_5",
			"IexMath-2_5",
			"IlmImf-2_5",
			"IlmImfUtil-2_5",
			"IlmThread-2_5",
			"Imath-2_5",
			"lzma",
			"jpeg",
			"libpng16",
			"tiff",
			"zlib",
			"lzma",
			"oiio",
			"Field3D",
			"NewtonSDK4",
			--cuda
			"cudart_static",
			"cuda",
			"nvrtc",
			"cudnn",
			"cublas",
			"curand",
			"cusolver",
			"cudart",
		}
		defines { "NDEBUG", "EIGEN_NO_DEBUG" }
		optimize "On"
		libdirs { THIRD_PARTY_DIR .. "builds/bin/" .. outputdir .. "/**",
				  THIRD_PARTY_DIR .. "precompiled/bin/" .. outputdir .. "/**",
				  REPO_ROOT .. "builds/bin/" .. outputdir .. "/**",
				  CUDA_LIB_DIR, CUDA_DNN_LIB_DIR,
		}
	
	  


	 		

