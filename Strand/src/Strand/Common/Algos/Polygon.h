#pragma once

#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>

namespace Strand::Common {

	using polygon_cut = std::tuple<std::vector<glm::vec2>, std::vector<uint32_t>, std::vector<glm::vec2>, std::vector<uint32_t>>;

	using polygon_crack = std::vector<std::pair<std::vector<glm::vec2>, std::vector<uint32_t>>>;

	using c_aabb2 = std::pair<glm::vec2, glm::vec2>;
	using c_aabb = std::pair<glm::vec3, glm::vec3>;

	template<typename C, typename T = std::decay_t<decltype(*begin(std::declval<C>()))>, typename = std::enable_if_t<std::is_same_v<T, glm::vec2>>>
	c_aabb2 GenPolygonBounds(
		const C& polygon)
	{
		c_aabb2 bounds(glm::vec2(FLT_MAX), glm::vec2(-FLT_MAX));
		auto& [min, max] = bounds;

		for (const glm::vec2& v : polygon)
		{
			if (v.x > max.x) max.x = v.x;
			if (v.x < min.x) min.x = v.x;
			if (v.y > max.y) max.y = v.y;
			if (v.y < min.y) min.y = v.y;
		}

		return bounds;
	}
}