#include "sdpch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer3D.h"
#include "Renderer2D.h"

namespace Strand
{
	struct RenderCommandData {
		Ref<Material> material;
		Ref<VertexArray> vertexArray;
		glm::mat4 transform;

		RenderCommandData(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
			: material(material), vertexArray(vertexArray), transform(transform) {
		}
	};

	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
	};

	struct RendererData
	{
		// Global Scene UBO
		Ref<UniformBuffer> SceneUniformBuffer;
		SceneData SceneBuffer;

		std::vector<RenderCommandData> CommandQueue;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		SD_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
		//I Understand now why these two lines stop renderer2D TODO THE UNIFORMBUFFER
		//s_Data.SceneUniformBuffer = UniformBuffer::Create(sizeof(SceneData), 0);
		//Renderer3D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		//Renderer3D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}


	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.SceneBuffer.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_Data.SceneUniformBuffer->SetData(&s_Data.SceneBuffer, sizeof(SceneData));

	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		SD_PROFILE_FUNCTION();

		s_Data.SceneBuffer.ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);
		s_Data.SceneUniformBuffer->SetData(&s_Data.SceneBuffer, sizeof(SceneData));
	}

	void Renderer::BeginScene(const EditorCamera& camera)
	{
		SD_PROFILE_FUNCTION();

		s_Data.SceneBuffer.ViewProjectionMatrix = camera.GetViewProjection();
		s_Data.SceneUniformBuffer->SetData(&s_Data.SceneBuffer, sizeof(SceneData));


	}

	void Renderer::EndScene()
	{
		Flush();
	}

	void Renderer::Submit(const Ref<Material>& material, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		s_Data.CommandQueue.emplace_back( material, vertexArray, transform );
	}

	void Renderer::Flush()
	{
		for (const auto& command : s_Data.CommandQueue)
		{
			/*command.Material->Bind();

			command.Material->GetShader()->SetMat4("u_Transform", command.Transform);

			command.VertexArray->Bind();
			RenderCommand::DrawIndexed(command.VertexArray);*/

		}
	}



	//void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	//{
	//	shader->Bind();
	//	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
	//	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

	//	vertexArray->Bind();
	//	RenderCommand::DrawIndexed(vertexArray);
	//}

}