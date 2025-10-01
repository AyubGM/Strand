#include "sdpch.h"
#include "Renderer3D.h"
#include "RenderCommand.h"

namespace Strand {

	struct DrawCommand
	{
		Ref<Mesh> Mesh;
		Ref<Material> Material;
		glm::mat4 Transform;

	};



	static Scope<SceneData> s_SceneData;

	struct Renderer3DData
	{
		Ref<Shader> LightCubeShader;

		std::vector<DrawCommand> OpaqueRenderQueue;

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

		struct SkyboxData {
			Ref<Shader> SkyboxShader;
			Ref<Mesh> CubeMesh;
			Ref<TextureCube> SkyboxTexture;
		} Skybox;

		CameraData CameraBuffer;
		ObjectData ObjectBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
		Ref<UniformBuffer> ObjectUniformBuffer; // binding = 1
		Ref<UniformBuffer> SceneUniformBuffer; // binding = 2

		Renderer3D::Statistics Stats;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
	
		SD_PROFILE_FUNCTION();
		s_SceneData = CreateScope<SceneData>();

		s_Data.LightCubeShader = Shader::Create("assets/shaders/Renderer3D_LightCube.glsl");
		s_Data.Skybox.SkyboxShader = Shader::Create("assets/shaders/Renderer3D_CubeMap.glsl");

		if (SD_DEBUG)
		{
			SD_CORE_INFO("Size of SceneData Struct In Renderer3D {0}", sizeof(SceneData));
			SD_CORE_INFO("Size of CameraData Struct In Renderer3D {0}", sizeof(Renderer3DData::CameraData));
			SD_CORE_INFO("Size of ObjectData Struct In Renderer3D {0}", sizeof(Renderer3DData::ObjectData));
		}

		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
		s_Data.ObjectUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::ObjectData), 1);
		s_Data.SceneUniformBuffer = UniformBuffer::Create(sizeof(SceneData), 2);

		std::vector<Strand::StaticMeshVertex> cubeVertices = {
	{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
	{glm::vec3(1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	{glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

	// Back face
	{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
	{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
	{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},

	// Right face
	{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
	{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

	// Left face
	{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
	{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

	// Top face
	{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
	{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

	// Bottom face
	{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
	{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
		};

		std::vector<uint32_t> cubeindces = {
			0, 1, 2,  // Front
			0, 2, 3,
			4, 5, 6,  // Back
			4, 6, 7,
			8, 9, 10, // Right
			8, 10, 11,
			12, 13, 14, // Left
			12, 14, 15,
			16, 17, 18, // Top
			16, 18, 19,
			20, 21, 22, // Bottom
			20, 22, 23
		};

		s_Data.Skybox.CubeMesh = Strand::CreateRef<Strand::Mesh>(cubeVertices, cubeindces);
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

	void Renderer3D::BeginScene(const EditorCamera& camera, const SceneData& sceneData)
	{
		SD_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));
		s_Data.SceneUniformBuffer->SetData(&sceneData, sizeof(SceneData));

		// Clear the queue for the new frame
		s_Data.OpaqueRenderQueue.clear();
		ResetStats();

	}

	void Renderer3D::EndScene()
	{
		SD_PROFILE_FUNCTION();
	}

	void Renderer3D::FlushOpaqueQueue()
	{
	
		SD_PROFILE_FUNCTION();

		std::sort(s_Data.OpaqueRenderQueue.begin(), s_Data.OpaqueRenderQueue.end(),
			[](const DrawCommand& a, const DrawCommand& b) {
				return a.Material->GetID() < b.Material->GetID();
			});

		Ref<Material> currentMaterail = nullptr;

		for (const auto& command : s_Data.OpaqueRenderQueue)
		{
			if (command.Material != currentMaterail)
			{
				command.Material->Bind();
				currentMaterail = command.Material;
			}

			s_Data.ObjectBuffer.u_Model = command.Transform;
			s_Data.ObjectBuffer.u_NormalMatrix = glm::transpose(glm::inverse(command.Transform));
			s_Data.ObjectUniformBuffer->SetData(&s_Data.ObjectBuffer, sizeof(Renderer3DData::ObjectBuffer));

			RenderCommand::DrawIndexed(command.Mesh->GetVertexArray());

			s_Data.Stats.DrawCalls++;
			s_Data.Stats.MeshCount++;
		}
			
		
	}

	void Renderer3D::Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const glm::mat4& transform)
	{
		SD_PROFILE_FUNCTION();
		if (!mesh || !material) return;

		//  Add a command to the Queue.
		s_Data.OpaqueRenderQueue.emplace_back(DrawCommand{ mesh, material, transform });
	}

	void Renderer3D::BeginSkyboxPass(const Ref<TextureCube>& skyboxTexture)
	{
		SD_PROFILE_FUNCTION();

		s_Data.Skybox.SkyboxTexture = skyboxTexture;

		RenderCommand::SetDepthFunc(RendererAPI::DepthFunc::LessEqual);
		RenderCommand::SetDepthMask(false);
	}

	void Renderer3D::SubmitSkybox()
	{
		SD_PROFILE_FUNCTION();
		if (!s_Data.Skybox.SkyboxTexture || !s_Data.Skybox.CubeMesh) return;

		s_Data.Skybox.SkyboxShader->Bind();
		s_Data.Skybox.SkyboxTexture->Bind(0); 

		RenderCommand::DrawIndexed(s_Data.Skybox.CubeMesh->GetVertexArray());

		s_Data.Stats.DrawCalls++;
	}

	void Renderer3D::EndSkyboxPass()
	{
		SD_PROFILE_FUNCTION();

		RenderCommand::SetDepthMask(true);
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunc::Less);
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

		RenderCommand::DrawIndexed(mesh->GetVertexArray());

		// Update performance statistics.
		s_Data.Stats.DrawCalls++;
		s_Data.Stats.MeshCount++;
	}

	 void Renderer3D::DrawCubeMap( const Ref<Mesh> mesh, const Ref<TextureCube> texture, const Ref<Shader> shader)
	{
		SD_PROFILE_FUNCTION();

		if (!mesh) return;
		
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunc::LessEqual);
		RenderCommand::SetDepthMask(false);

		shader->Bind();
		texture->Bind(5);

		RenderCommand::DrawIndexed(mesh->GetVertexArray());

		RenderCommand::SetDepthMask(true);
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunc::Less);

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