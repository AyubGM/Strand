#include "sdpch.h"
#include "Mesh.h"

namespace Strand {


	Mesh::Mesh(const std::vector<StaticMeshVertex>& vertices, const std::vector<uint32_t>& indices)
		: m_IndexCount(indices.size())
	{
		m_VertexArray = VertexArray::Create();


		m_VertexBuffer = VertexBuffer::Create((float*)vertices.data(), vertices.size() * sizeof(StaticMeshVertex));

		m_VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		m_IndexBuffer = IndexBuffer::Create((uint32_t*)indices.data(), m_IndexCount);

		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

}