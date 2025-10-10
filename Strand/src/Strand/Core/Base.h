#pragma once

#include <memory>
#include "Strand/Core/PlatformDetection.h"

#ifdef SD_DEBUG
#if defined(SD_PLATFORM_WINDOWS)
#define SD_DEBUGBREAK() __debugbreak()
#elif defined(SD_PLATFORM_LINUX)
#include <signal.h>
#define SD_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define SD_ENABLE_ASSERTS
#else
#define SD_DEBUGBREAK()
#endif

#define SD_EXPAND_MACRO(x) x
#define SD_STRINGIFY_MACRO(x) #x

//TODO

#define BIT(x) (1 << x)
#define SD_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
//#define SD_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Strand {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Strand/Core/Log.h"
#include "Strand/Core/Assert.h"
#include "Strand/Core/Common.h"




//#ifdef SD_PLATFORM_WINDOWS
//#if SD_DYNAMIC_LINK
//	#ifdef SD_BUILD_DLL
//		#define STRAND_API __declspec(dllexport)
//	#else
//		#define STRAND_API __declspec(dllimport)
//	#endif // SD_BUILD_DLL
//
//#else
//   #define STRAND_API
//#endif // SD_DYNAMIC_LINK
//#else
//	#error Strand only supports Windows!
//#endif // SD_PLATFORM_WINDOWS