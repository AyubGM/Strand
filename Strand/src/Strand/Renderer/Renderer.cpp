#include "sdpch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer3D.h"
#include "Renderer2D.h"

namespace Strand
{
	struct RenderCommandData {
		Ref<Material> Material;
		Ref<VertexArray> VertexArray;
		glm::mat4 Transform;

		RenderCommandData(const Ref<Strand::Material>& material, const Ref<Strand::VertexArray>& vertexArray, const glm::mat4& transform)
			: Material(material), VertexArray(vertexArray), Transform(transform) {
		}
	};

	struct SceneDataDebug
	{
		glm::mat4 ViewProjectionMatrix;
	};

	struct RendererData
	{
		// Global Scene UBO
		Ref<UniformBuffer> SceneUniformBuffer;
		SceneDataDebug SceneBuffer;

		Scope<MaterialLibrary> Library;

		std::vector<RenderCommandData> CommandQueue;
	};

	static RendererData s_Data;

	void Renderer::Init()
	{
		SD_PROFILE_FUNCTION();

		RenderCommand::Init();
		s_Data.SceneUniformBuffer = UniformBuffer::Create(sizeof(SceneDataDebug), 0);
		Renderer2D::Init();
		//I Understand now why these two lines stop renderer2D TODO THE UNIFORMBUFFER
		
		//Renderer3D::Init();
		s_Data.Library = CreateScope<MaterialLibrary>();
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
		SD_PROFILE_FUNCTION();
		s_Data.SceneBuffer.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_Data.SceneUniformBuffer->SetData(&s_Data.SceneBuffer, sizeof(SceneDataDebug));
		s_Data.CommandQueue.clear();

	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		SD_PROFILE_FUNCTION();

		s_Data.SceneBuffer.ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);
		s_Data.SceneUniformBuffer->SetData(&s_Data.SceneBuffer, sizeof(SceneDataDebug));
		s_Data.CommandQueue.clear();
	}

	void Renderer::BeginScene(const EditorCamera& camera)
	{
		SD_PROFILE_FUNCTION();

		s_Data.SceneBuffer.ViewProjectionMatrix = camera.GetViewProjection();
		s_Data.SceneUniformBuffer->SetData(&s_Data.SceneBuffer, sizeof(SceneDataDebug));
		s_Data.CommandQueue.clear();

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
			command.Material->Bind();

			command.Material->GetShader()->SetMat4("u_Transform", command.Transform);

			RenderCommand::DrawIndexed(command.VertexArray);

		}
	}

	void Renderer::AddMaterial(const std::string& name, const Ref<Material>& material)
	{
		s_Data.Library->Add(name, material);
	}

	Ref<Material> Renderer::GetMaterial(const std::string& name)
	{
		return s_Data.Library->Get(name);
	}


	//void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	//{
	//	shader->Bind();
	//	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneDataDebug->ViewProjectionMatrix);
	//	std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

	//	vertexArray->Bind();
	//	RenderCommand::DrawIndexed(vertexArray);
	//}

}