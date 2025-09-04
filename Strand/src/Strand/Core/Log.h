#pragma once


#include "Strand/Core/Base.h"


#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)


namespace Strand
{
	class Log
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

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define SD_CORE_TRACE(...)		::Strand::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SD_CORE_INFO(...)		::Strand::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SD_CORE_WARN(...)		::Strand::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SD_CORE_ERROR(...)		::Strand::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SD_CORE_CRITICAL(...)	::Strand::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SD_TRACE(...)			::Strand::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SD_INFO(...)			::Strand::Log::GetClientLogger()->info(__VA_ARGS__)
#define SD_WARN(...)			::Strand::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SD_ERROR(...)			::Strand::Log::GetClientLogger()->error(__VA_ARGS__)
#define SD_CRITICAL(...)		::Strand::Log::GetClientLogger()->critical(__VA_ARGS__)