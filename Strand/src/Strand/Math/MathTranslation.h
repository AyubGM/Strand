#pragma once

#include "Strand/Math/Matrix.h"

namespace Strand::Math
{
	inline vec<3> From_glm(glm::vec3 v) 
	{
		vec<3> result;

		result.set(0, v.x);
		result.set(1, v.y);
		result.set(2, v.z);

		return result;
	}

	inline mat<3, 3> From_glm(glm::mat3 v) 
	{
		mat<3, 3> result;

		for (int i = 0; i < 9; i++) result.elements[i] = v[i / 3][i % 3];

		return result;
	}

	inline glm::vec3 To_glm(vec<3> vec) 
	{
		return glm::vec3(vec.get(0), vec.get(1), vec.get(2));
	}

	inline glm::vec2 To_glm(vec<2> vec) {
		return glm::vec2(vec.get(0), vec.get(1));
	}

	inline float CrossLength(glm::vec2 a, glm::vec2 b) { // should move to geom 2d
		return a.x * b.y - a.y * b.x;
	}

}