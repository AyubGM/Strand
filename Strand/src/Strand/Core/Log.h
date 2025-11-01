#pragma once


#include "Strand/Core/Base.h"

#include <filesystem>

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

template <>
struct fmt::formatter<glm::vec3> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const glm::vec3& v, FormatContext& ctx) const -> decltype(ctx.out()) {
		// Output the vector in a simple (x, y, z) format.
		// Adjust precision or format as desired.
		return fmt::format_to(ctx.out(), "({0:.2f}, {1:.2f}, {2:.2f})", v.x, v.y, v.z);
	}
};

template <>
struct fmt::formatter<glm::vec4> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const glm::vec4& v, FormatContext& ctx) const -> decltype(ctx.out()) {
		// Output the vector in a simple (x, y, z) format.
		// Adjust precision or format as desired.
		return fmt::format_to(ctx.out(), "({0:.2f}, {1:.2f}, {2:.2f}, {3:.2f})", v.x, v.y, v.z, v.w);
	}

};

//TODO CHECK WHY IS NOT WORKING
template <> 
struct fmt::formatter<std::filesystem::path> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const std::filesystem::path& path, FormatContext& ctx) const -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{}", path.string());
	}
};

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