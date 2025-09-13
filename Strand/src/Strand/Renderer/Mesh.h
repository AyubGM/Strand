#pragma once

#include <memory>
#include "Strand/Renderer/VertexArray.h"

namespace Strand {
	struct StaticMeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<StaticMeshVertex>& vertices, const std::vector<uint32_t>& indices);

		~Mesh() = default;

		//virtual void Bind() const = 0;
		//virtual void Unbind() const = 0;

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		// The number of indices in the mesh.
		uint32_t m_IndexCount = 0;
	};

}