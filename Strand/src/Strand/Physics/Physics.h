#pragma once

#include "glm/glm.hpp"

inline float Major(glm::vec2& v) {
	float m = v.x;
	if (v.y > m) m = v.y;
	return m;
}

inline float Major(glm::vec3& v) {
	float m = v.x;
	if (v.y > m) m = v.y;
	if (v.z > m) m = v.z;
	return m;
}

inline float Minor(glm::vec3& v) {
	float m = v.x;
	if (v.y < m) m = v.y;
	if (v.z < m) m = v.z;
	return m;
}

namespace Strand::Physics {

	template<Dimension D>
	_vec<D> TransformPoint(const _vec<D>& point, TransformComponent* transform)
	{
		glm::vec4 v;

		if constexpr (D == D2) v = glm::vec4(point, 0, 1);
		else                    v = glm::vec4(point, 1);

		return _vec<D>(v * glm::transpose(transform->GetTransform()));
	}
}