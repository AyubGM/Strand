#include "../UniformBufferT.h"
#include <glad/glad.h>

namespace Strand {

	UniformBufferT::~UniformBufferT() {
		if (m_RendererID) {
			glDeleteBuffers(1, &m_RendererID);
		}
	}

	UniformBufferT::UniformBufferT(UniformBufferT&& other) noexcept
		: m_RendererID(other.m_RendererID), m_Size(other.m_Size), m_Binding(other.m_Binding)
	{
		other.m_RendererID = 0;
		other.m_Size = 0;
		other.m_Binding = 0;
	}

	UniformBufferT& UniformBufferT::operator=(UniformBufferT&& other) noexcept {
		if (this != &other) {
			if (m_RendererID) glDeleteBuffers(1, &m_RendererID);

			m_RendererID = other.m_RendererID;
			m_Size = other.m_Size;
			m_Binding = other.m_Binding;

			other.m_RendererID = 0;
			other.m_Size = 0;
			other.m_Binding = 0;
		}
		return *this;
	}

	void UniformBufferT::Init(uint32_t size, uint32_t binding) {
		m_Size = size;
		m_Binding = binding;
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void UniformBufferT::SetData(const void* data, uint32_t size, uint32_t offset) {
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}

	void UniformBufferT::Bind(uint32_t binding) const {
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	void UniformBufferT::Unbind() const {
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

} // namespace Strand