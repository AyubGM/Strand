#include "../IndexBufferT.h"
#include <glad/glad.h>

namespace Strand {

	IndexBufferT::~IndexBufferT() {
		if (m_RendererID) {
			glDeleteBuffers(1, &m_RendererID);
		}
	}

	IndexBufferT::IndexBufferT(IndexBufferT&& other) noexcept
		: m_RendererID(other.m_RendererID), m_Count(other.m_Count)
	{
		other.m_RendererID = 0;
		other.m_Count = 0;
	}

	IndexBufferT& IndexBufferT::operator=(IndexBufferT&& other) noexcept {
		if (this != &other) {
			if (m_RendererID) glDeleteBuffers(1, &m_RendererID);

			m_RendererID = other.m_RendererID;
			m_Count = other.m_Count;

			other.m_RendererID = 0;
			other.m_Count = 0;
		}
		return *this;
	}

	void IndexBufferT::Init(uint32_t* indices, uint32_t count) {
		m_Count = count;
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	void IndexBufferT::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBufferT::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} // namespace Strand