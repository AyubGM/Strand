#pragma once

#include "Strand/Scene/Components.h"

#include <utility>

namespace Strand {

	using c_aabb2 = std::pair<glm::vec2, glm::vec2>;
	using c_aabb = std::pair<glm::vec3, glm::vec3>;


	inline c_aabb2 TransformBounds(const c_aabb2& bounds, const TransformComponent* transform) { return c_aabb2(); }

	inline c_aabb TransformBounds(const c_aabb& bounds, const TransformComponent* transform) { return c_aabb(); }
}