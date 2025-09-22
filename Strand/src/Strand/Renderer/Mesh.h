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

	struct StaticMeshTexture {
		uint32_t id;
		std::string type;
	};

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::vector<StaticMeshVertex>& vertices, const std::vector<uint32_t>& indices);
		Mesh(const std::vector<StaticMeshVertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<StaticMeshTexture> textures);
		//void Draw(Shader& shader);

		~Mesh() = default;

		std::vector<StaticMeshVertex>  m_Vertices;
		std::vector<uint32_t> m_Indices;
		std::vector<StaticMeshTexture> m_Textures;


		//virtual void Bind() const = 0;
		//virtual void Unbind() const = 0;

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
	private:
		void setupMesh();

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		// The number of indices in the mesh.
		uint32_t m_IndexCount = 0;
	};

}