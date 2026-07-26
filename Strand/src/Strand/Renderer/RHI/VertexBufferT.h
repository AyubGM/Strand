#pragma once
#include "BufferLayout.h"

namespace Strand {

	enum class BufferUsage : uint8_t {
		Static = 0,
		Dynamic,
		Stream
	};

	class VertexBufferT
	{
	public:
		~VertexBufferT() {}

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32_t size);


		const BufferLayout& GetLayout() const;
		void SetLayout(const BufferLayout& layout);
	};

}