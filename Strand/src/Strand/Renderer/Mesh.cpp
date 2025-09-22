#include "sdpch.h"
#include "Mesh.h"

namespace Strand {


	Mesh::Mesh(const std::vector<StaticMeshVertex>& vertices, const std::vector<uint32_t>& indices)
		: m_IndexCount(indices.size())
	{
		setupMesh();
	}

	Mesh::Mesh(const std::vector<StaticMeshVertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<StaticMeshTexture> textures)
		: m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
	{
		setupMesh();
	}

	void Mesh::setupMesh()
	{
		m_VertexArray = VertexArray::Create();


		m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(StaticMeshVertex));

		m_VertexBuffer->SetLayout({
		   { ShaderDataType::Float3, "a_Position" },
		   { ShaderDataType::Float3, "a_Normal"},
		   { ShaderDataType::Float2, "a_TexCoord"}
			});

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_IndexCount);

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

}