#pragma once

#include "Strand/Core/Base.h"

namespace Strand {

	class UniformBufferT
	{
	public:
		~UniformBufferT() {}
		void SetData(const void* data, uint32_t size, uint32_t offset = 0);
	};

}