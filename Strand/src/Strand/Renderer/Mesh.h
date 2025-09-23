#pragma once

#include <memory>
#include "Strand/Renderer/VertexArray.h"

namespace Strand {
	struct StaticMeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		//glm::vec3 Tangent;
		//glm::vec3 Bitangent;
	};

	struct StaticMeshTexture {
		uint32_t id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:

		Mesh() = default;
		Mesh(const std::vector<StaticMeshVertex>& vertices, const std::vector<uint32_t>& indices);
		Mesh(std::vector<StaticMeshVertex>&& vertices, std::vector<uint32_t>&& indices, uint32_t materialIndex);

		~Mesh() = default;

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const uint32_t GetMaterialIndex() const { return m_MaterialIndex; }


	private:
		void setupMesh();

	private:
		// CPU-side data
		std::vector<StaticMeshVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;

		// GPU resources
		Ref<VertexArray> m_VertexArray;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		// Link to the material used by this mesh
		uint32_t m_MaterialIndex;
	};

}