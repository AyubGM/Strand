#pragma once
#include "BufferLayoutT.h"

namespace Strand {

	enum class BufferUsage : uint8_t {
		Static = 0,
		Dynamic,
		Stream
	};

	class VertexBufferT
	{
	public:
		VertexBufferT() = default;
		~VertexBufferT() {}

		VertexBufferT(const VertexBufferT&) = delete;
		VertexBufferT& operator=(const VertexBufferT&) = delete;

		VertexBufferT(VertexBufferT&& other) noexcept;
		VertexBufferT& operator=(VertexBufferT&& other) noexcept;

		void Init(uint32_t size, BufferUsage usage = BufferUsage::Dynamic, const void* data = nullptr);
		void Bind() const;
		void Unbind() const;
		void SetData(const void* data, uint32_t size, uint32_t offset = 0);

		const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

		[[nodiscard]] uint32_t GetRendererID() const { return m_RendererID; }
		[[nodiscard]] uint32_t GetSize() const { return m_Size; }

		operator bool() const { return m_RendererID != 0; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Size = 0;
		BufferLayout m_Layout;
	};

} // namespace Strand