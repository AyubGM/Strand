#include "sdpch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"

namespace Strand {




	struct SceneData {
		glm::vec3 CameraPosition;
		int NumPointLights;
		PointLight PointLights[4];
		DirectLight DirectLight;
		Spotlight Spotlight;
	};

	static Scope<SceneData> s_SceneData;

	struct Renderer3DData
	{
		Ref<Shader> PBRShader;
		Ref<Shader> DefualtShader;
		Ref<Shader> LightCubeShader;

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

		s_Data.PBRShader = Shader::Create("assets/shaders/Renderer3D_PBR.glsl");
		s_Data.DefualtShader = Shader::Create("assets/shaders/Renderer3D_Defualt.glsl");
		s_Data.LightCubeShader = Shader::Create("assets/shaders/Renderer3D_LightCube.glsl");

		if (SD_DEBUG)
		{
			SD_CORE_INFO("Size of SceneData Struct In Renderer3D {0}", sizeof(SceneData));
			SD_CORE_INFO("Size of CameraData Struct In Renderer3D {0}", sizeof(Renderer3DData::CameraData));
			SD_CORE_INFO("Size of ObjectData Struct In Renderer3D {0}", sizeof(Renderer3DData::ObjectData));
		}


		// Create a default white texture for untextured materials.
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
		s_Data.ObjectUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::ObjectData), 1);
		//TODO USe MetrialDATA for PBR
		//s_Data.MaterialUniformBuffer = UniformBuffer::Create(sizeof(MaterialData), 2);
		s_Data.MaterialUniformBuffer = UniformBuffer::Create(sizeof(MaterialD), 2);
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

		s_SceneData->CameraPosition = camera.GetPosition();
		glm::vec4 postion = glm::vec4(1.2f, 1.0f, 2.0f, 1.0f);
		glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		glm::vec4 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		s_SceneData->PointLights[0] = {postion, ambient, diffuse, specular };
		s_Data.SceneUniformBuffer->SetData(s_SceneData.get(), sizeof(SceneData));

	}

	// The new BeginScene function for setting up scene-wide data like lights.
	void Renderer3D::BeginScene(const EditorCamera& camera, const std::vector<PointLight>& pointLights, const DirectLight& directLight, const Spotlight& spotLight)
	{

		SD_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));

		
		s_SceneData->CameraPosition = camera.GetPosition();
		s_SceneData->NumPointLights = glm::min((uint32_t)pointLights.size(), 4u);
		for (uint32_t i = 0; i < s_SceneData->NumPointLights; ++i) {
			s_SceneData->PointLights[i] = pointLights[i];
		}
		s_SceneData->DirectLight = directLight;
		s_SceneData->Spotlight = spotLight;
		s_Data.SceneUniformBuffer->SetData(s_SceneData.get(), sizeof(SceneData));

	}

	void Renderer3D::EndScene()
	{
		SD_PROFILE_FUNCTION();
	}

	void Renderer3D::DrawMesh(const glm::mat4& transform, const Ref<Mesh> mesh, const Ref<Material> material)
	{
		SD_PROFILE_FUNCTION();

		if (!mesh || !material) return;

		material->Bind();

		// Set the per-object data (transform matrices)
		s_Data.ObjectBuffer.u_Model = transform;
		s_Data.ObjectBuffer.u_NormalMatrix = glm::transpose(glm::inverse(transform));
		s_Data.ObjectBuffer.u_ObjectColor = glm::vec3(1);
		// We no longer set u_ObjectColor here, the material handles all appearance
		s_Data.ObjectUniformBuffer->SetData(&s_Data.ObjectBuffer, sizeof(Renderer3DData::ObjectBuffer));

		RenderCommand::DrawIndexed(mesh->GetVertexArray());

		// Update performance statistics.
		s_Data.Stats.DrawCalls++;
		s_Data.Stats.MeshCount++;
	}



	void Renderer3D::DrawCubeMesh(const glm::mat4& transform, const Ref<Mesh> mesh, const glm::vec3& cubeColor, const Ref<Material> materail)
	{
		SD_PROFILE_FUNCTION();

		if (!mesh) return;
		
		materail->Bind();

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

	 void Renderer3D::DrawCubeMap( const Ref<Mesh> mesh, const Ref<Texture3D> texture, const Ref<Shader> shader)
	{
		SD_PROFILE_FUNCTION();

		if (!mesh) return;

		shader->Bind();
		texture->Bind();

		RenderCommand::DrawIndexed(mesh->GetVertexArray());

		// Update performance statistics.
		s_Data.Stats.DrawCalls++;
		s_Data.Stats.MeshCount++;
	}

	 void Renderer3D::DrawLightCube(const glm::mat4& transform, const Ref<Mesh> mesh, const glm::vec3& cubeColor)
	 {
		 SD_PROFILE_FUNCTION();

		 if (!mesh) return;

		 s_Data.LightCubeShader->Bind();

		 // Update UBOs
		 s_Data.ObjectBuffer.u_Model = transform;
		 s_Data.ObjectBuffer.u_NormalMatrix = glm::transpose(glm::inverse(transform));
		 s_Data.ObjectBuffer.u_ObjectColor = cubeColor;
		 s_Data.ObjectUniformBuffer->SetData(&s_Data.ObjectBuffer, sizeof(Renderer3DData::ObjectBuffer));

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