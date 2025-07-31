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

#ifdef GM_ENABLE_ASSERTS
#define GM_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define GM_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define GM_ASSERT(x, ...)
#define GM_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)
#define GM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
