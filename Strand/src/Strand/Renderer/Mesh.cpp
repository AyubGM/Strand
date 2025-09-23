#include "sdpch.h"
#include "Mesh.h"

namespace Strand {


	Mesh::Mesh(const std::vector<StaticMeshVertex>& vertices, const std::vector<uint32_t>& indices)
	{
		setupMesh();
	}

	Mesh::Mesh(std::vector<StaticMeshVertex>&& vertices, std::vector<uint32_t>&& indices, uint32_t materialIndex)
		: m_Vertices(std::move(vertices)), m_Indices(std::move(indices)), m_MaterialIndex(materialIndex)
	{
		// Calculate metadata before setting up GPU buffers
		//CalculateBoundingBox();
		setupMesh();

		// Optional: If you don't need CPU-side data anymore, you can clear it to save memory.
		// m_Vertices.clear();
		// m_Indices.clear();
	}

	void Mesh::setupMesh()
	{
		m_VertexArray = VertexArray::Create();


		m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(StaticMeshVertex));

		m_VertexBuffer->SetLayout({
		   { ShaderDataType::Float3, "a_Position" },
		   { ShaderDataType::Float3, "a_Normal"},
		   { ShaderDataType::Float2, "a_TexCoord"},
		   //{ ShaderDataType::Float3, "a_Tangent" },
		   //{ ShaderDataType::Float3, "a_Bitangent"}
			});

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer = IndexBuffer::Create((uint32_t*)m_Indices.data(), m_Indices.size());

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		
	}

}