#pragma once

namespace Strand {

	class StorageBuffer
	{
	public:
		StorageBuffer() = default;
		~StorageBuffer();

		StorageBuffer(const StorageBuffer&) = delete;
		StorageBuffer& operator=(const StorageBuffer&) = delete;

		StorageBuffer(StorageBuffer&& other) noexcept;
		StorageBuffer& operator=(StorageBuffer&& other) noexcept;

		void Init(uint32_t size, const void* data = nullptr);
		void SetData(const void* data, uint32_t size, uint32_t offset = 0);

		void Bind(uint32_t binding) const;
		void Unbind() const;

		void BindRange(uint32_t bindingPoint, size_t offset, size_t size);
		void Unbind();

		[[nodiscard]] uint32_t GetRendererID() const { return m_RendererID; }
		[[nodiscard]] uint32_t GetSize() const { return m_Size; }

		operator bool() const { return m_RendererID != 0; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Size = 0;
	};

}