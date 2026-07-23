project "gtest"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"googletest/src/**.cc",
		"googletest/src/**.h",
		"googlemock/src/**.cc",
		"googlemock/src/**.h",
	}

	includedirs
	{
		"googletest",
		"googletest/include",
		"googlemock",
		"googlemock/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"