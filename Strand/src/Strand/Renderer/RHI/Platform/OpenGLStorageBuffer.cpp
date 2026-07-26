#include "../StorageBuffer.h"
#include <glad/glad.h>

namespace Strand {
	

	StorageBuffer::~StorageBuffer() {
		if (m_RendererID) {
			glDeleteBuffers(1, &m_RendererID);
		}
	}

	StorageBuffer::StorageBuffer(StorageBuffer&& other) noexcept
		: m_RendererID(other.m_RendererID), m_Size(other.m_Size)
	{
		other.m_RendererID = 0;
		other.m_Size = 0;
	}

	StorageBuffer& StorageBuffer::operator=(StorageBuffer&& other) noexcept {
		if (this != &other) {
			// Delete existing buffer first if we already have one
			if (m_RendererID) glDeleteBuffers(1, &m_RendererID);

			m_RendererID = other.m_RendererID;
			m_Size = other.m_Size;

			// Invalidate the other object so it doesn't delete the handle
			other.m_RendererID = 0;
			other.m_Size = 0;
		}
		return *this;
	}

	void StorageBuffer::Init(uint32_t size, const void* data) {
		m_Size = size;
		glCreateBuffers(1, &m_RendererID);
		// GL_DYNAMIC_DRAW is best here since you update RenderObjects frequently
		glNamedBufferData(m_RendererID, size, data, GL_DYNAMIC_DRAW);
	}

	void StorageBuffer::Bind(uint32_t slot) const {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RendererID);
	}

	void StorageBuffer::Unbind() const {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void StorageBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}

}