#include <Strand.h>
#include <Strand/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Strand::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray = Strand::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Strand::Ref<Strand::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Strand::VertexBuffer::Create(vertices, sizeof(vertices)));
		Strand::BufferLayout layout = {
			{ Strand::ShaderDataType::Float3, "a_Position" },
			{ Strand::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Strand::Ref<Strand::IndexBuffer> indexBuffer;
		indexBuffer.reset(Strand::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Strand::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		Strand::Ref<Strand::VertexBuffer> squareVB;
		squareVB.reset(Strand::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Strand::ShaderDataType::Float3, "a_Position" },
			{ Strand::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Strand::Ref<Strand::IndexBuffer> squareIB;
		squareIB.reset(Strand::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Strand::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);


		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader = Strand::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);



		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Strand::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Strand::Texture2D::Create("assets/textures/ChernoLogo.png");

	}

	void OnUpdate(Strand::Timestep ts) override
	{


		Strand::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		Strand::RenderCommand::Clear();

		// Update
		m_CameraController.OnUpdate(ts);

		Strand::Renderer::BeginScene(m_CameraController.GetCamera());


		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		std::dynamic_pointer_cast<Strand::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Strand::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		glm::vec4 redColor(0.8, 0.2, 0.3, 1.0);
		glm::vec4 blueColor(0.2, 0.3, 0.8, 1.0);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Strand::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Strand::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		Strand::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Strand::Renderer::Submit(m_Shader, m_VertexArray);


		Strand::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Strand::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}
	bool OnKeyPressed(Strand::KeyPressedEvent& event)
	{
	//	if (event.GetKeyCode() == SD_KEY_ESCAPE)
	//	{
	//		//Strand::Application::Get().GetWindow().Close();
	//		return true;
	//	}
	//	if (event.GetKeyCode() == SD_KEY_LEFT)
	//		m_CameraPosition.x -= m_CameraSpeed;

	//	if (event.GetKeyCode() == SD_KEY_RIGHT)
	//		m_CameraPosition.x += m_CameraSpeed;

	//	if (event.GetKeyCode() == SD_KEY_DOWN)
	//		m_CameraPosition.y -= m_CameraSpeed;

	//	if (event.GetKeyCode() == SD_KEY_UP)
	//		m_CameraPosition.y += m_CameraSpeed;


	//	return false;
	}

private:
	Strand::ShaderLibrary m_ShaderLibrary;
	Strand::Ref<Strand::Shader> m_Shader;
	Strand::Ref<Strand::VertexArray> m_VertexArray;

	Strand::Ref<Strand::Shader> m_FlatColorShader;
	Strand::Ref<Strand::VertexArray> m_SquareVA;

	Strand::Ref<Strand::Texture2D> m_Texture, m_ChernoLogoTexture;

	//Strand::OrthographicCamera m_Camera;

	Strand::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };


};
class Sandbox : public Strand::Application
{
public:
	Sandbox()
	{ 
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{
	}


};

Strand::Application* Strand::CreateApplication()
{
	return new Sandbox();
}