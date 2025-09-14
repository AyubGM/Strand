#pragma once

#include "Strand/Core/Base.h"
#include "Strand/Renderer/VertexArray.h"
#include "Strand/Renderer/Shader.h"
#include "Strand/Renderer/Texture.h"
#include "Strand/Renderer/UniformBuffer.h"
#include "Strand/Renderer/Camera.h"
#include "Strand/Renderer/EditorCamera.h"
#include "Strand/Renderer/Mesh.h"

namespace Strand {

	struct PointLight
	{
		glm::vec3 Position;
		glm::vec3 Color;
	};

	struct Material
	{
		Ref<Texture2D> AlbedoMap;
		Ref<Texture2D> NormalMap;
		Ref<Texture2D> MetallicMap;
		Ref<Texture2D> RoughnessMap;
		Ref<Texture2D> AOMap;
	};

	class Renderer3D
	{
	public:
		// Initializes the 3D renderer's data, including vertex buffers, shaders, and other resources.
		static void Init();
		// Cleans up all the resources allocated by the renderer.
		static void Shutdown();

		// Begins the 3D scene, setting up the camera and starting a new render batch.
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const EditorCamera& camera, const std::vector<PointLight>& pointLights);

		// Ends the 3D scene and flushes the remaining render data to the GPU.
		static void EndScene();

		// Draws a static mesh.
		//static void DrawStaticMesh(const glm::mat4& transform, Ref<Mesh> mesh, Ref<Shader> shader);
		static void DrawCubeMesh(const glm::mat4& transform, const Ref<Mesh> mesh, const glm::vec3& cubeColor, const PointLight& light);
		static void DrawStaticMesh(const glm::mat4& transform, Ref<Mesh> mesh, Ref<Material> material);
		static void DrawStaticMesh(const glm::mat4& transform, Ref<Mesh> mesh, const glm::vec3& albedo, float metallic, float roughness, float ao);


		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			//uint32_t VertexCount = 0;
			//uint32_t IndexCount = 0;
			uint32_t MeshCount = 0;

			uint32_t GetTotalVertexCount() { return MeshCount * 8; }
			uint32_t GetTotalIndexCount() { return MeshCount * 36; }
			uint32_t GetTotalMeshCount() { return MeshCount; }
		};
		static void ResetStats();
		static Statistics GetStats();
	private:
	
	/*	struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;*/

		// Flushes the current render batch to the GPU.
		static void Flush();
		// Starts a new render batch after the current one is full.
		static void NextBatch();

	};

}