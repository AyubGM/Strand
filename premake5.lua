workspace "Strand"
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
IncludeDir["GLFW"] = "Strand/vendor/GLFW/include"
IncludeDir["Glad"] = "Strand/vendor/Glad/include"
IncludeDir["ImGui"] = "Strand/vendor/imgui"
IncludeDir["glm"] = "Strand/vendor/glm"



group "Dependencies"
	include "Strand/vendor/GLFW"
	include "Strand/vendor/Glad"
	include "Strand/vendor/imgui"

project "Strand"
	location "Strand"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "sdpch.h"
	pchsource "Strand/src/sdpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"

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
			"SD_PLATFORM_WINDOWS",
			"SD_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			 ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}


		filter "configurations:Debug"
			defines "SD_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "SD_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "SD_DIST"
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
		"Strand/vendor/spdlog/include",
		"Strand/src",
		"Strand/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Strand"
	}

	filter "system:windows"
		cppdialect "C++20"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines
		{
			"SD_PLATFORM_WINDOWS",
		}

	

		filter "configurations:Debug"
			defines "SD_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "SD_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "SD_DIST"
			runtime "Release"
			optimize "On"