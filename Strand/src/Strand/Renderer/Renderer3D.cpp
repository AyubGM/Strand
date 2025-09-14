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

	static Scope<SceneData> s_SceneData;

	struct Renderer3DData
	{
		Ref<Shader> PBRShader;
		Ref<Shader> PBRSimpleShader;
		Ref<Shader> PBRTexturedShader;
		Ref<Shader> DefualtShader;

		// Default white texture
		Ref<Texture2D> WhiteTexture;


		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		struct ObjectData
		{
			glm::mat4 u_Model;
			glm::mat4 u_NormalMatrix;
			glm::vec3 u_ObjectColor;

		};

		CameraData CameraBuffer;
		ObjectData ObjectBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
		Ref<UniformBuffer> ObjectUniformBuffer; // binding = 1
		Ref<UniformBuffer> MaterialUniformBuffer; // binding = 2
		Ref<UniformBuffer> SceneUniformBuffer; // binding = 3

		Renderer3D::Statistics Stats;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		SD_PROFILE_FUNCTION();
		s_SceneData = CreateScope<SceneData>();

		s_Data.PBRShader = Shader::Create("assets/shaders/Renderer3D_PBR_Simple.glsl");
		s_Data.PBRSimpleShader = Shader::Create("assets/shaders/Renderer3D_PBR_Simple.glsl");
		s_Data.PBRTexturedShader = Shader::Create("assets/shaders/Renderer3D_PBR_Textured.glsl");
		s_Data.DefualtShader = Shader::Create("assets/shaders/Renderer3D_Defualt.glsl");

		// Create a default white texture for untextured materials.
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

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

	}

	// The new BeginScene function for setting up scene-wide data like lights.
	void Renderer3D::BeginScene(const EditorCamera& camera, const std::vector<PointLight>& pointLights)
	{
		SD_PROFILE_FUNCTION();

		//s_Data.PBRSimpleShader->Bind();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		
		s_SceneData->CameraPosition = camera.GetPosition();
		s_SceneData->NumPointLights = glm::min((uint32_t)pointLights.size(), 4u);
		for (uint32_t i = 0; i < s_SceneData->NumPointLights; ++i) {
			s_SceneData->PointLights[i] = pointLights[i];
		}
		s_Data.SceneUniformBuffer->SetData(s_SceneData.get(), sizeof(SceneData));

	}

	void Renderer3D::EndScene()
	{
		SD_PROFILE_FUNCTION();
	}

	void Renderer3D::DrawCubeMesh(const glm::mat4& transform, const Ref<Mesh> mesh, glm::vec3& cubeColor, glm::vec3& LightColor)
	{
		SD_PROFILE_FUNCTION();

		if (!mesh) return;

		s_Data.DefualtShader->Bind();

		s_SceneData->PointLights[0].Color = LightColor;
		s_Data.SceneUniformBuffer->SetData(s_SceneData.get(), sizeof(SceneData));
		// Update UBOs
		s_Data.ObjectBuffer.u_Model = transform;
		s_Data.ObjectBuffer.u_NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ObjectBuffer.u_ObjectColor = cubeColor;
		s_Data.ObjectUniformBuffer->SetData(&s_Data.ObjectBuffer, sizeof(Renderer3DData::ObjectBuffer));

		mesh->GetVertexArray()->Bind();
		RenderCommand::DrawIndexed(mesh->GetVertexArray());

		// Update performance statistics.
		s_Data.Stats.DrawCalls++;
		s_Data.Stats.MeshCount++;
	}

	void Renderer3D::DrawStaticMesh(const glm::mat4& transform, const Ref<Mesh> mesh,
		const glm::vec3& albedo, float metallic, float roughness, float ao)
	{
		SD_PROFILE_FUNCTION();

		if (!mesh) return;

		s_Data.PBRSimpleShader->Bind();

		s_Data.ObjectBuffer.u_Model = transform;
		s_Data.ObjectBuffer.u_NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ObjectUniformBuffer->SetData(&s_Data.ObjectBuffer, sizeof(Renderer3DData::ObjectBuffer));

		MaterialData matData = { albedo, metallic, roughness, ao };
		s_Data.MaterialUniformBuffer->SetData(&matData, sizeof(MaterialData));

		mesh->GetVertexArray()->Bind();
		RenderCommand::DrawIndexed(mesh->GetVertexArray());

		s_Data.Stats.DrawCalls++;
		s_Data.Stats.MeshCount++;
	}


	void Renderer3D::DrawStaticMesh( const glm::mat4& transform, const Ref<Mesh> mesh, const Ref<Material> material)
	{
		SD_PROFILE_FUNCTION();

		s_Data.PBRTexturedShader->Bind();

		// Update UBOs
		s_Data.ObjectBuffer.u_Model = transform;
		s_Data.ObjectBuffer.u_NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ObjectUniformBuffer->SetData(&s_Data.ObjectBuffer, sizeof(Renderer3DData::ObjectBuffer));

		// Use preprocessor directive uniform to switch between textured and simple PBR.
		bool hasTextures = material && material->AlbedoMap;
		s_Data.PBRShader->SetInt("u_HasTextures", hasTextures ? 1 : 0);

		// Bind textures to their respective texture units
		material->AlbedoMap->Bind(0);
		material->NormalMap->Bind(1);
		material->MetallicMap->Bind(2);
		material->RoughnessMap->Bind(3);
		material->AOMap->Bind(4);

		// Set the sampler uniform integers to the correct texture units
		s_Data.PBRTexturedShader->SetInt("u_AlbedoMap", 0);
		s_Data.PBRTexturedShader->SetInt("u_NormalMap", 1);
		s_Data.PBRTexturedShader->SetInt("u_MetallicMap", 2);
		s_Data.PBRTexturedShader->SetInt("u_RoughnessMap", 3);
		s_Data.PBRTexturedShader->SetInt("u_AOMap", 4);

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