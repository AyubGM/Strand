#include <Strand.h>
#include <ImGui/imgui.h>

class ExampleLayer : public Strand::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f, 0.0f, 0.0f)
	{
		m_VertexArray.reset(Strand::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Strand::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Strand::VertexBuffer::Create(vertices, sizeof(vertices)));
		Strand::BufferLayout layout = {
			{ Strand::ShaderDataType::Float3, "a_Position" },
			{ Strand::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Strand::IndexBuffer> indexBuffer;
		indexBuffer.reset(Strand::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Strand::VertexArray::Create());

		float squareVertices[3 * 4] = {
					-0.75f, -0.75f, 0.0f,
					 0.75f, -0.75f, 0.0f,
					 0.75f,  0.75f, 0.0f,
					-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Strand::VertexBuffer> squareVB;
		squareVB.reset(Strand::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Strand::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Strand::IndexBuffer> squareIB;
		squareIB.reset(Strand::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
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

		m_Shader.reset(new Strand::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Strand::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnUpdate(Strand::Timestep ts) override
	{


		if (Strand::Input::IsKeyPressed(SD_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Strand::Input::IsKeyPressed(SD_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Strand::Input::IsKeyPressed(SD_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Strand::Input::IsKeyPressed(SD_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Strand::Input::IsKeyPressed(SD_KEY_A))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		else if (Strand::Input::IsKeyPressed(SD_KEY_D))
			m_CameraRotation += m_CameraRotationSpeed * ts;

	

		Strand::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		Strand::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Strand::Renderer::BeginScene(m_Camera);


		Strand::Renderer::Submit(m_BlueShader, m_SquareVA);
		Strand::Renderer::Submit(m_Shader, m_VertexArray);


		Strand::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Example Layer");
		ImGui::Text("Hello from ExampleLayer!");
		if (ImGui::Button("Click Me"))
			SD_TRACE("Button clicked!");
		ImGui::End();
	}

	void OnEvent(Strand::Event& event) override
	{
		/*Strand::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Strand::KeyPressedEvent>(SD_BIND_EVENT_FN(ExampleLayer::OnKeyPressed));*/

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
	std::shared_ptr<Strand::Shader> m_Shader;
	std::shared_ptr<Strand::VertexArray> m_VertexArray;

	std::shared_ptr<Strand::Shader> m_BlueShader;
	std::shared_ptr<Strand::VertexArray> m_SquareVA;

	Strand::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 280.0f;

};
class Sandbox : public Strand::Application
{
public:
	Sandbox()
	{ 
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}


};

Strand::Application* Strand::CreateApplication()
{
	return new Sandbox();
}