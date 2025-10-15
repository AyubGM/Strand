#pragma once

#include <glm/glm.hpp>

namespace Strand::Math {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

	template<typename T>
	const T& Max(const T& a, const T& b)
	{
		return a >= b ? a : b;
	}
}

