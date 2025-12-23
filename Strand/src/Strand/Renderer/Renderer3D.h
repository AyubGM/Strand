#pragma once

#include "Strand/Core/Base.h"
#include "Strand/Renderer/VertexArray.h"
#include "Strand/Renderer/Shader.h"
#include "Strand/Renderer/Texture.h"
#include "Strand/Renderer/UniformBuffer.h"
#include "Strand/Renderer/Camera.h"
#include "Strand/Renderer/EditorCamera.h"
#include "Strand/Renderer/Material.h"
#include "Strand/Renderer/Mesh.h"

namespace Strand {

	struct PointLight
	{
		glm::vec4 Position;
		glm::vec4 Ambient;
		glm::vec4 Diffuse;
		glm::vec4 Specular;
		float Constant;
		float Linear;
		float Quadratic;
		float _padding_1;
	};

	struct DirectLight
	{
		glm::vec4 Direction;
		glm::vec4 Ambient;
		glm::vec4 Diffuse;
		glm::vec4 Specular;
	};

	struct Spotlight
	{
		glm::vec4 Position;
		glm::vec4 Direction;
		glm::vec4 Ambient;
		glm::vec4 Diffuse;
		glm::vec4 Specular;
		float CutOff;
		float OuterCutOff;
		float Constant;
		float Linear;
		float Quadratic;

	};


	struct SceneData {
		glm::vec3 CameraPosition;
		int NumPointLights;
		std::array<PointLight, 4> PointLights;
		DirectLight DirectLight;
		Spotlight Spotlight;
	};

	class Renderer3D
	{
	public:

		static void Init();
		static void Shutdown();

		static void BeginScene(const EditorCamera& camera, const SceneData& sceneData);

		static void EndScene();

		// Draws a static mesh.
		static void Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const glm::mat4& transform);
		static void SubmitInstanced(const Ref<Mesh>& mesh, const Ref<Material>& material, const std::vector<glm::mat4>& transforms);
		

		static void BeginSkyboxPass(const Ref<TextureCube>& skyboxTexture);
		static void SubmitSkybox();
		static void EndSkyboxPass();

		//TODO REMOVE
		static void DrawLightCube(const glm::mat4& transform, const Ref<Mesh> mesh, const glm::vec3& cubeColor);
		static void DrawCubeMesh(const glm::mat4& transform, const Ref<Mesh> mesh, const glm::vec3& cubeColor, const Ref<Material> materail);
		static void DrawCubeMap(const Ref<Mesh> mesh, const Ref<TextureCube> texture, const Ref<Shader> shader);
		static void DrawMesh(const glm::mat4& transform, const Ref<Mesh> mesh, const Ref<Material> material);

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

		static void FlushOpaqueQueue();
		static void NextBatch();

	};

}