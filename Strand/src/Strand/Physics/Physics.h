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