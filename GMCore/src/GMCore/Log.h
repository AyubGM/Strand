#pragma once


#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace GMCore
{
	class GMCORE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};


}

// Core log macros
#define GM_CORE_TRACE(...)		::GMCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GM_CORE_INFO(...)		::GMCore::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GM_CORE_WARN(...)		::GMCore::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GM_CORE_ERROR(...)		::GMCore::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GM_CORE_FATAL(...)		::GMCore::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define GM_TRACE(...)			::GMCore::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GM_INFO(...)			::GMCore::Log::GetClientLogger()->info(__VA_ARGS__)
#define GM_WARN(...)			::GMCore::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GM_ERROR(...)			::GMCore::Log::GetClientLogger()->error(__VA_ARGS__)
#define GM_FATAL(...)			::GMCore::Log::GetClientLogger()->critical(__VA_ARGS__)