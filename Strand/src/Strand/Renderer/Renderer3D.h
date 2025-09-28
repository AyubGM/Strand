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

		float _padding_1;
		float _padding_2;
		float _padding_3;
	};


	struct MaterialD {
		//glm::vec4 ambient;
		//Ref<Texture2D> diffuse;
		//glm::vec4 specular;
		float shininess;
	};

	class Renderer3D
	{
	public:

		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const EditorCamera& camera, const std::vector<PointLight>& pointLights, const DirectLight& directLight, const Spotlight& spotLight);

		static void EndScene();

		// Draws a static mesh.
		//static void DrawStaticMesh(const glm::mat4& transform, Ref<Mesh> mesh, Ref<Shader> shader);
		static void DrawLightCube(const glm::mat4& transform, const Ref<Mesh> mesh, const glm::vec3& cubeColor);
		static void DrawCubeMesh(const glm::mat4& transform, const Ref<Mesh> mesh, const glm::vec3& cubeColor, const Ref<Material> materail);
		static void DrawCubeMap( const Ref<Mesh> mesh, const Ref<Texture3D> texture, const Ref<Shader> shader);
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
	
	/*	struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;*/

		static void Flush();
		static void NextBatch();

	};

}