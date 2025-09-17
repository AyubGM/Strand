#pragma once

#include "Strand/Renderer/RenderCommand.h"
#include "Strand/Renderer/Camera.h"
#include "Strand/Renderer/EditorCamera.h"
#include "Strand/Renderer/OrthographicCamera.h"
#include "Strand/Renderer/Shader.h"
#include "Strand/Renderer/Mesh.h"
#include "Strand/Renderer/Material.h"

namespace Strand
{

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();


		static void Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:

		

		// SecneData So we do not have to copy the whole camera class
		//struct SceneData
		//{
		//	glm::mat4 ViewProjectionMatrix;
		//};

		//static Scope<SceneData> s_SceneData;

	};

}