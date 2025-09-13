#include "sdpch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"

namespace Strand {



	struct Renderer3DData
	{
		Ref<Shader> PBRShader;
		Ref<Shader> DefualtShader;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;

		Renderer3D::Statistics Stats;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		SD_PROFILE_FUNCTION();

		//TODO PBR Shader
		s_Data.PBRShader = Shader::Create("assets/shaders/Renderer3D_PBR.glsl");

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
	}

	void Renderer3D::Shutdown()
	{
		SD_PROFILE_FUNCTION();
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		SD_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));
	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		SD_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		//StartBatch();
	}

	// The new BeginScene function for setting up scene-wide data like lights.
	void Renderer3D::BeginScene(const glm::vec3& cameraPosition, const std::vector<PointLight>& pointLights)
	{
		SD_PROFILE_FUNCTION();

		s_Data.PBRShader->Bind();
		s_Data.PBRShader->SetFloat3("u_CameraPosition", cameraPosition);

		// Set light data uniforms.
		const uint32_t numLights = glm::min((uint32_t)pointLights.size(), 4u); // Limit to 4 for now
		for (uint32_t i = 0; i < numLights; ++i)
		{
			s_Data.PBRShader->SetFloat3("u_PointLights[" + std::to_string(i) + "].Position", pointLights[i].Position);
			s_Data.PBRShader->SetFloat3("u_PointLights[" + std::to_string(i) + "].Color", pointLights[i].Color);
		}
		s_Data.PBRShader->SetInt("u_NumPointLights", numLights);
	}

	void Renderer3D::EndScene()
	{
		SD_PROFILE_FUNCTION();
	}

	void Renderer3D::DrawStaticMesh( const glm::mat4& transform, const Ref<Mesh> mesh, const glm::vec3& albedo, float metallic, float roughness, float ao)
	{
		SD_PROFILE_FUNCTION();

		s_Data.PBRShader->Bind();
		// Set the per-object uniforms.
		s_Data.PBRShader->SetMat4("u_Model", transform);
		s_Data.PBRShader->SetFloat3("u_Albedo", albedo);
		s_Data.PBRShader->SetFloat("u_Metallic", metallic);
		s_Data.PBRShader->SetFloat("u_Roughness", roughness);
		s_Data.PBRShader->SetFloat("u_AO", ao);


		mesh->GetVertexArray()->Bind();
		RenderCommand::DrawIndexed(mesh->GetVertexArray());

		// Update performance statistics.
		s_Data.Stats.DrawCalls++;
		s_Data.Stats.MeshCount++;
	}

	//-------------------------------------------------------------------------------------------------
	// Statistics
	//-------------------------------------------------------------------------------------------------

	void Renderer3D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_Data.Stats;
	}

}