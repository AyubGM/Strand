#include "sdpch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"

namespace Strand {



	// Material
	struct MaterialData {
		glm::vec3 Albedo;
		float Metallic;
		float Roughness;
		float AO;
	};

	struct SceneData {
		glm::vec3 CameraPosition;
		int NumPointLights;
		PointLight PointLights[4];
	};

	struct Renderer3DData
	{
		Ref<Shader> PBRShader;
		Ref<Shader> DefualtShader;


		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		struct ObjectData
		{
			glm::mat4 u_Model;
			glm::mat4 u_NormalMatrix;

		};

		CameraData CameraBuffer;
		ObjectData ObjectBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
		Ref<UniformBuffer> ObjectUniformBuffer; // binding = 1
		Ref<UniformBuffer> MaterialUniformBuffer; // binding = 2
		Ref<UniformBuffer> SceneUniformBuffer; // binding = 

		Renderer3D::Statistics Stats;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		SD_PROFILE_FUNCTION();

		s_Data.PBRShader = Shader::Create("assets/shaders/Renderer3D_PBR.glsl");
		s_Data.DefualtShader = Shader::Create("assets/shaders/Renderer3D_Defualt.glsl");

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
		s_Data.ObjectUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::ObjectData), 1);
		s_Data.MaterialUniformBuffer = UniformBuffer::Create(sizeof(MaterialData), 2);
		s_Data.SceneUniformBuffer = UniformBuffer::Create(sizeof(SceneData), 3);
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
	void Renderer3D::BeginScene(const EditorCamera& camera, const std::vector<PointLight>& pointLights)
	{
		SD_PROFILE_FUNCTION();

		s_Data.PBRShader->Bind();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		SceneData sceneData = {};
		sceneData.CameraPosition = camera.GetPosition();
		sceneData.NumPointLights = glm::min((uint32_t)pointLights.size(), 4u);
		for (uint32_t i = 0; i < sceneData.NumPointLights; ++i) {
			sceneData.PointLights[i] = pointLights[i];
		}
		s_Data.SceneUniformBuffer->SetData(&sceneData, sizeof(SceneData));
	}

	void Renderer3D::EndScene()
	{
		SD_PROFILE_FUNCTION();
	}

	void Renderer3D::DrawCubeMesh(const glm::mat4& transform, const Ref<Mesh> mesh)
	{
		SD_PROFILE_FUNCTION();

		s_Data.DefualtShader->Bind();
		// Update UBOs
		s_Data.ObjectBuffer.u_Model = transform;
		s_Data.ObjectBuffer.u_NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ObjectUniformBuffer->SetData(&s_Data.ObjectBuffer, sizeof(Renderer3DData::ObjectBuffer));

		mesh->GetVertexArray()->Bind();
		RenderCommand::DrawIndexed(mesh->GetVertexArray());

		// Update performance statistics.
		s_Data.Stats.DrawCalls++;
		s_Data.Stats.MeshCount++;
	}

	void Renderer3D::DrawStaticMesh( const glm::mat4& transform, const Ref<Mesh> mesh, const glm::vec3& albedo, float metallic, float roughness, float ao)
	{
		SD_PROFILE_FUNCTION();

		s_Data.PBRShader->Bind();
		// Update UBOs
		s_Data.ObjectBuffer.u_Model = transform;
		s_Data.ObjectBuffer.u_NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ObjectUniformBuffer->SetData(&s_Data.ObjectBuffer, sizeof(Renderer3DData::ObjectBuffer));
		MaterialData matData = { albedo, metallic, roughness, ao };
		s_Data.MaterialUniformBuffer->SetData(&matData, sizeof(MaterialData));

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