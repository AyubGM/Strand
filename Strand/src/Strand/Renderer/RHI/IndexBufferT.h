#pragma once

namespace Strand {

	class IndexBufferT
	{
	public:
		IndexBufferT() = default;
		~IndexBufferT();

		IndexBufferT(const IndexBufferT&) = delete;
		IndexBufferT& operator=(const IndexBufferT&) = delete;

		IndexBufferT(IndexBufferT&& other) noexcept;
		IndexBufferT& operator=(IndexBufferT&& other) noexcept;

		void Init(uint32_t* indices, uint32_t count);
		void Bind() const;
		void Unbind() const;

		[[nodiscard]] uint32_t GetCount() const { return m_Count; }
		[[nodiscard]] uint32_t GetRendererID() const { return m_RendererID; }

		operator bool() const { return m_RendererID != 0; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Count = 0;
	};

} // namespace Strand