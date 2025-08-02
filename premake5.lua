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
IncludeDir["Glad"] = "GMCore/vendor/Glad/include"
IncludeDir["ImGui"] = "GMCore/vendor/imgui"



group "Dependencies"
	include "GMCore/vendor/GLFW"
	include "GMCore/vendor/Glad"
	include "GMCore/vendor/imgui"

project "GMCore"
	location "GMCore"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"

	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"dwmapi.lib"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines
		{
			"GM_PLATFORM_WINDOWS",
			"GM_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			 ("{COPY} %{cfg.buildtarget.abspath} ../bin/" .. outputdir .. "/Sandbox")
		}


		filter "configurations:Debug"
			defines "GM_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "GM_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "GM_DIST"
			runtime "Release"
			optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines
		{
			"GM_PLATFORM_WINDOWS",
		}

	

		filter "configurations:Debug"
			defines "GM_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "GM_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "GM_DIST"
			runtime "Release"
			optimize "On"