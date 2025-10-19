#pragma once

#include <glm/glm.hpp>

namespace Strand::Math {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

	template<typename T>
	const T& Max(const T& a, const T& b)
	{
		return a >= b ? a : b;
	}

	template<typename T>
	const T& Min(const T& a, const T& b)
	{
		return a <= b ? a : b;
	}

	constexpr float Pi = 3.14159265359f;
	constexpr float Pi2 = Pi * 2;
	constexpr float hPi = Pi * 0.5f;
	constexpr float e = 2.71828182846f;
	constexpr float Phi = 1.61803398875f;

	template<typename T> // should T be a ref or not const refds
	T Clamp(const T& x, const T& min, const T& max)
	{
		if (min > x) return min;
		if (max < x) return max;

		return x;
	}


	template<typename T>
	T Norm(const T& vec)
	{
		if (vec == T(0.f))
		{
			return vec;
		}

		return glm::normalize(vec);
	}
}

