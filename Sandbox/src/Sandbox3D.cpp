#include "Sandbox3D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>




glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f), m_EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f), m_SquareColor({0.2f, 0.3f, 0.8f, 1.0f})
{
	std::vector<Strand::StaticMeshVertex> cubeVertices = {
		// Vertex 0: Front-bottom-left
		{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		// Vertex 1: Front-bottom-right
		{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		// Vertex 2: Front-top-right
		{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		// Vertex 3: Front-top-left
		{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

		// Vertex 4: Back-bottom-left
		{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		// Vertex 5: Back-bottom-right
		{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		// Vertex 6: Back-top-right
		{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
		// Vertex 7: Back-top-left
		{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)}
	};

	std::vector<uint32_t> cubeindces = {
		0, 1, 2,  // Front face, first triangle
		2, 3, 0,  // Front face, second triangle

		1, 5, 6,  // Right face, first triangle
		6, 2, 1,  // Right face, second triangle

		5, 4, 7,  // Back face, first triangle
		7, 6, 5,  // Back face, second triangle

		4, 0, 3,  // Left face, first triangle
		3, 7, 4,  // Left face, second triangle

		3, 2, 6,  // Top face, first triangle
		6, 7, 3,  // Top face, second triangle

		4, 5, 1,  // Bottom face, first triangle
		1, 0, 4   // Bottom face, second triangle
	};

	//m_CubeMesh = Strand::Mesh(cubeVertices, cubeindces);
	m_CubeMesh = Strand::CreateRef<Strand::Mesh>(cubeVertices, cubeindces);
}

void Sandbox3D::OnAttach()
{
	SD_PROFILE_FUNCTION();

	m_CheckerboardTexture = Strand::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox3D::OnDetach()
{
	SD_PROFILE_FUNCTION();
}

void Sandbox3D::OnUpdate(Strand::Timestep ts)
{
	SD_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);


	// Render
	Strand::Renderer3D::ResetStats();
	{
		SD_PROFILE_SCOPE("Renderer Prep");
		Strand::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Strand::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		std::vector<Strand::PointLight> lights = {
			{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) }
		};

		SD_PROFILE_SCOPE("Renderer Draw");
		//Strand::Renderer3D::BeginScene(m_EditorCamera, lights);
		//Strand::Renderer3D::DrawStaticMesh(model, m_CubeMesh, glm::vec3(0.5f, 0.3f, 0.4f), 1.0f, 0.5f, 1.0f);
		//Strand::Renderer3D::EndScene();

		Strand::Renderer3D::BeginScene(m_EditorCamera);
		Strand::Renderer3D::DrawCubeMesh(model, m_CubeMesh);
		Strand::Renderer3D::EndScene();

		m_EditorCamera.OnUpdate(ts);
		
	}

}

void Sandbox3D::OnImGuiRender()
{
	SD_PROFILE_FUNCTION();



	ImGui::Begin("Settings");

	auto stats = Strand::Renderer3D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	//ImGui::Text("Quads: %d", stats.QuadCount);
	//ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	//ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

}




void Sandbox3D::OnEvent(Strand::Event& e)
{
	m_CameraController.OnEvent(e);
}