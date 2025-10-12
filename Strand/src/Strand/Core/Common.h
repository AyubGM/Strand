#pragma once

#include <glm/glm.hpp>

namespace Strand {

	enum class Dimension {
		D2 = 2,
		D3 = 3,
	};

	template<Dimension D>
	using _vec = glm::vec<uint32_t(D), float>;

	template<Dimension D>
	using _ivec = glm::vec<uint32_t(D), int>;

}


