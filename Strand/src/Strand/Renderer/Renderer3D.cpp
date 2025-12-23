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

	struct InstancedDrawCommand
	{
		Ref<Mesh> Mesh;
		Ref<Material> Material;
		uint32_t InstanceCount = 0;
	};


	static Scope<SceneData> s_SceneData;

	struct Renderer3DData
	{

		Ref<UniformBuffer> CameraUniformBuffer;
		Ref<UniformBuffer> ObjectUniformBuffer; // binding = 1
		Ref<UniformBuffer> SceneUniformBuffer; // binding = 2

		Ref<Shader> LightCubeShader;

		std::vector<DrawCommand> OpaqueRenderQueue;

		std::vector<InstancedDrawCommand> OpaqueInstancedRenderQueue;

		Ref<VertexBuffer> InstancedDataBuffer;
		static const uint32_t MaxInstances = 10000;
		static const uint32_t MaxInstanceDataSize = MaxInstances * sizeof(glm::mat4);

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
		//TODO FIX THE BINDING
		s_Data.SceneUniformBuffer = UniformBuffer::Create(sizeof(SceneData), 3);

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

		s_Data.InstancedDataBuffer = VertexBuffer::Create(s_Data.MaxInstanceDataSize);
		s_Data.InstancedDataBuffer->SetLayout({
			{ ShaderDataType::Mat4, "a_InstanceMatrix", false, 1 },
			});
	}

	void Renderer3D::Shutdown()
	{
		SD_PROFILE_FUNCTION();
	}

	void Renderer3D::BeginScene(const EditorCamera& camera, const SceneData& sceneData)
	{
		SD_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer3DData::CameraData));
		s_Data.SceneUniformBuffer->SetData(&sceneData, sizeof(SceneData));

		// Clear the queue for the new frame
		s_Data.OpaqueRenderQueue.clear();
		s_Data.OpaqueInstancedRenderQueue.clear();
		ResetStats();

	}

	void Renderer3D::EndScene()
	{
		SD_PROFILE_FUNCTION();

		FlushOpaqueQueue();
	}

	void Renderer3D::FlushOpaqueQueue()
	{
	
		SD_PROFILE_FUNCTION();

		std::sort(s_Data.OpaqueRenderQueue.begin(), s_Data.OpaqueRenderQueue.end(),
			[](const DrawCommand& a, const DrawCommand& b) {
				return a.Material->GetID() < b.Material->GetID();
			});

		Ref<Material> currentMaterial = nullptr;

		for (const auto& command : s_Data.OpaqueRenderQueue)
		{
			if (command.Material != currentMaterial)
			{
				command.Material->Bind();
				currentMaterial = command.Material;
			}

			s_Data.ObjectBuffer.u_Model = command.Transform;
			s_Data.ObjectBuffer.u_NormalMatrix = glm::transpose(glm::inverse(command.Transform));
			s_Data.ObjectUniformBuffer->SetData(&s_Data.ObjectBuffer, sizeof(Renderer3DData::ObjectBuffer));

			RenderCommand::DrawIndexed(command.Mesh->GetVertexArray());

			s_Data.Stats.DrawCalls++;
			s_Data.Stats.MeshCount++;
		}

		std::sort(s_Data.OpaqueInstancedRenderQueue.begin(), s_Data.OpaqueInstancedRenderQueue.end(),
			[](const InstancedDrawCommand& a, const InstancedDrawCommand& b) {
				return a.Material->GetID() < b.Material->GetID();
				/*if (a.Material->GetID() != b.Material->GetID())
					return a.Material->GetID() < b.Material->GetID();*/
				//return a.Mesh->GetVertexArray()->GetRendererID() < b.Mesh->GetVertexArray()->GetRendererID();
			});

		Ref<Material> currentInstancedMaterial = nullptr;
		for (const auto& command : s_Data.OpaqueInstancedRenderQueue)
		{

			if (command.Material != currentInstancedMaterial)
			{
				command.Material->Bind();
				currentInstancedMaterial = command.Material;
			}


			RenderCommand::DrawIndexedInstanced(command.Mesh->GetVertexArray(), command.InstanceCount);

			s_Data.Stats.DrawCalls++;
			s_Data.Stats.MeshCount += command.InstanceCount;
		}
		
	}

	void Renderer3D::Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const glm::mat4& transform)
	{
		SD_PROFILE_FUNCTION();
		if (!mesh || !material) return;

		//  Add a command to the Queue.
		s_Data.OpaqueRenderQueue.emplace_back(DrawCommand{ mesh, material, transform });
	}


	

	void Renderer3D::SubmitInstanced(const Ref<Mesh>& mesh, const Ref<Material>& material, const std::vector<glm::mat4>& transforms)
	{
		SD_PROFILE_FUNCTION();
		if (transforms.empty()) return;

		uint32_t instanceCount = static_cast<uint32_t>(transforms.size());
		if (instanceCount > s_Data.MaxInstances)
		{
			SD_CORE_WARN("Instancing batch full! Dropping {0} transforms.", instanceCount);
			return;
		}

		// Enable instancing for this mesh if not already done
		mesh->EnableInstancing(instanceCount);

		mesh->UpdateInstanceData(transforms.data(), instanceCount * sizeof(glm::mat4));

		// Submit mesh for instanced rendering using its own instance buffer
		s_Data.OpaqueInstancedRenderQueue.push_back({ mesh, material, instanceCount });
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
		s_Data.Skybox.SkyboxTexture->Bind(5); 

		RenderCommand::DrawIndexed(s_Data.Skybox.CubeMesh->GetVertexArray());

		s_Data.Stats.DrawCalls++;
		s_Data.Stats.MeshCount++;
	}

	void Renderer3D::EndSkyboxPass()
	{
		SD_PROFILE_FUNCTION();

		RenderCommand::SetDepthMask(true);
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunc::Less);
	}



	//TODO REMOVE

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

	void Renderer3D::DrawCubeMap(const Ref<Mesh> mesh, const Ref<TextureCube> texture, const Ref<Shader> shader)
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