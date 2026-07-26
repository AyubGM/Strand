#include "../VertexBufferT.h"
#include <glad/glad.h>

namespace Strand {

	VertexBufferT::~VertexBufferT()
	{
		if (m_RendererID)
		{
			glDeleteBuffers(1, &m_RendererID);
		}
	}

	VertexBufferT::VertexBufferT(VertexBufferT&& other) noexcept
		: m_RendererID(other.m_RendererID), m_Size(other.m_Size), m_Layout(std::move(other.m_Layout))
	{
		other.m_RendererID = 0;
		other.m_Size = 0;
	}

	VertexBufferT& VertexBufferT::operator=(VertexBufferT&& other) noexcept
	{
		if (this != &other)
		{
			if (m_RendererID)
			{
				glDeleteBuffers(1, &m_RendererID);
			}
			m_RendererID = other.m_RendererID;
			m_Size = other.m_Size;
			m_Layout = std::move(other.m_Layout);
			other.m_RendererID = 0;
			other.m_Size = 0;
		}

		return *this;
	}

	void VertexBufferT::Init(uint32_t size, BufferUsage usage, const void* data) {
		m_Size = size;
		GLenum glUsage = GL_DYNAMIC_DRAW;
		switch (usage) {
		case BufferUsage::Static:  glUsage = GL_STATIC_DRAW; break;
		case BufferUsage::Dynamic: glUsage = GL_DYNAMIC_DRAW; break;
		case BufferUsage::Stream:  glUsage = GL_STREAM_DRAW; break;
		}

		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, data, glUsage);
	}

	void VertexBufferT::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBufferT::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBufferT::SetData(const void* data, uint32_t size, uint32_t offset) {
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}


}