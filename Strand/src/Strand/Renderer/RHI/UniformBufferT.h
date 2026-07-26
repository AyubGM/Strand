#pragma once

namespace Strand {

	class UniformBufferT
	{
	public:
		UniformBufferT() = default;
		~UniformBufferT();

		UniformBufferT(const UniformBufferT&) = delete;
		UniformBufferT& operator=(const UniformBufferT&) = delete;

		UniformBufferT(UniformBufferT&& other) noexcept;
		UniformBufferT& operator=(UniformBufferT&& other) noexcept;

		void Init(uint32_t size, uint32_t binding);
		void SetData(const void* data, uint32_t size, uint32_t offset = 0);
		void Bind(uint32_t binding) const;
		void Unbind() const;

		[[nodiscard]] uint32_t GetRendererID() const { return m_RendererID; }
		[[nodiscard]] uint32_t GetSize() const { return m_Size; }
		[[nodiscard]] uint32_t GetBinding() const { return m_Binding; }

		operator bool() const { return m_RendererID != 0; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Size = 0;
		uint32_t m_Binding = 0;
	};

} // namespace Strand