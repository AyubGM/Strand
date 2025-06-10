#pragma once

#ifdef GM_PLATFORM_WINDOWS
	#ifdef GM_BUILD_DLL
		#define GMCORE_API __declspec(dllexport)
	#else
		#define GMCORE_API __declspec(dllimport)
	#endif // GM_BUILD_DLL

#else
#error GM only support Windows!
#endif // GM_PLATFORM_WINDOWS

#define BIT(x) (1 << x)
