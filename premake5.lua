workspace "GMCore"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "GMCore/vendor/GLFW/include"

include "GMCore/vendor/GLFW"

project "GMCore"
	location "GMCore"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gmcpch.h"
	pchsource "GMCore/src/gmcpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links 
	{ 
		"GLFW",
		"opengl32.lib",
		"dwmapi.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines
		{
			"GM_PLATFORM_WINDOWS",
			"GM_BUILD_DLL" 
		}

		postbuildcommands
		{
			 ("{COPY} %{cfg.buildtarget.abspath} ../bin/" .. outputdir .. "/Sandbox")
		}


		filter "configurations:Debug"
			defines "GM_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "GM_RELEASE"
			optimize "On"

		filter "configurations:Dist"
			defines "GM_DIST"
			optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"GMCore/vendor/spdlog/include",
		"GMCore/src"
	}

	links
	{
		"GMCore"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines
		{
			"GM_PLATFORM_WINDOWS",
		}

	

		filter "configurations:Debug"
			defines "GM_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "GM_RELEASE"
			optimize "On"

		filter "configurations:Dist"
			defines "GM_DIST"
			optimize "On"