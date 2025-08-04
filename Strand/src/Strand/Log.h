#pragma once


#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Strand
{
	class STRAND_API Log
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
#define SD_CORE_TRACE(...)		::Strand::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SD_CORE_INFO(...)		::Strand::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SD_CORE_WARN(...)		::Strand::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SD_CORE_ERROR(...)		::Strand::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SD_CORE_FATAL(...)		::Strand::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SD_TRACE(...)			::Strand::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SD_INFO(...)			::Strand::Log::GetClientLogger()->info(__VA_ARGS__)
#define SD_WARN(...)			::Strand::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SD_ERROR(...)			::Strand::Log::GetClientLogger()->error(__VA_ARGS__)
#define SD_FATAL(...)			::Strand::Log::GetClientLogger()->critical(__VA_ARGS__)