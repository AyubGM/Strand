workspace "Strand"
	architecture "x64"
	startproject "Bridges"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


--  Strand Dependencies --------

-- Include directories relative to root folder (solution directory)
VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "Strand/vendor/GLFW/include"
IncludeDir["Glad"] = "Strand/vendor/Glad/include"
IncludeDir["ImGui"] = "Strand/vendor/imgui"
IncludeDir["glm"] = "Strand/vendor/glm"
IncludeDir["Box2D"] = "Strand/vendor/Box2D/include"
IncludeDir["stb_image"] = "Strand/vendor/stb_image"
IncludeDir["entt"] = "Strand/vendor/entt/include"
IncludeDir["mono"] = "Strand/vendor/mono/include"
IncludeDir["yaml_cpp"] = "Strand/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Strand/vendor/ImGuizmo"
IncludeDir["shaderc"] = "Strand/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "Strand/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["assimp"] = "Strand/vendor/assimp/include"


-----VULKAN------------
LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_DebugDLL"] = "%{wks.location}/Strand/vendor/VulkanSDK/Bin"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
---------------------

-----ASSIMP------------
LibraryDir["assimp"] = "vendor/assimp"
Library["assimp_Debug"] = "%{LibraryDir.assimp}/lib/Debug/assimp-vc143-mtd.lib"
Library["zlibstaticd_Debug"] = "%{LibraryDir.assimp}/contrib/zlib//Debug/zlibstaticd.lib"

---MONO-------------------------
LibraryDir["mono"] = "vendor/mono/lib/%{cfg.buildcfg}"
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"


-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"

--  Strand Dependencies --------

group "Dependencies"
    include "Strand/vendor/Box2D"
	include "Strand/vendor/GLFW"
	include "Strand/vendor/Glad"
	include "Strand/vendor/imgui"
	include "Strand/vendor/yaml-cpp"

group ""

project "Strand"
	location "Strand"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "sdpch.h"
	pchsource "Strand/src/sdpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",

		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE",
		"IMGUI_DEFINE_MATH_OPERATORS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.assimp}",

	}

	links 
	{ 
		"Box2D",
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",
		"%{Library.mono}",
		
	}

	filter "files:Strand/vendor/ImGuizmo/**.cpp"
    flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }

		defines
		{

		}

			links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}",
		}


		filter "configurations:Debug"
			defines "SD_DEBUG"
			runtime "Debug"
			symbols "on"

			links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}",
			"%{Library.assimp_Debug}",
			"%{Library.zlibstaticd_Debug}"
			
		}

		filter "configurations:Release"
			defines "SD_RELEASE"
			runtime "Release"
			optimize "on"

			links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

		filter "configurations:Dist"
			defines "SD_DIST"
			runtime "Release"
			optimize "on"


		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
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
		"%{IncludeDir.Box2D}",
		"Strand/src",
		"Strand/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.assimp}",
	}

	links
	{
		"Box2D",
		"Strand"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }
	

		filter "configurations:Debug"
			defines "SD_DEBUG"
			runtime "Debug"
			symbols "on"

	 postbuildcommands
	{
		"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
	}

		filter "configurations:Release"
			defines "SD_RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "SD_DIST"
			runtime "Release"
			optimize "on"


project "Bridges"
	location "Bridges"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
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
		"%{IncludeDir.Box2D}",
		"Strand/src",
		"Strand/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}",
	}

	links
	{
		"Box2D",
		"Strand"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions { "/utf-8" }
		
	filter "configurations:Debug"
		defines "SD_DEBUG"
		runtime "Debug"
		symbols "on"

    postbuildcommands
	{
		"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
	}

	filter "configurations:Release"
		defines "SD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SD_DIST"
		runtime "Release"
		optimize "on"


include "Strand-ScriptCore"
