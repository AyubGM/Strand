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
	: Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f), m_EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f)
{
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

	//m_CubeMesh = Strand::Mesh(cubeVertices, cubeindces);
	m_CubeMesh = Strand::CreateRef<Strand::Mesh>(cubeVertices, cubeindces);
}

void Sandbox3D::OnAttach()
{
	SD_PROFILE_FUNCTION();

	m_CheckerboardTexture = Strand::Texture2D::Create("assets/textures/Checkerboard.png");
	m_Diffuse = Strand::Texture2D::Create("assets/textures/container2.png");
	m_Specular = Strand::Texture2D::Create("assets/textures/container2_specular.png");
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
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		/*std::vector<Strand::PointLight> lights = {
			{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) }
		};*/

		glm::vec4 postion = glm::vec4(1.2f, 1.0f, 2.0f, 1.0f);
		glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		glm::vec4 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		
		Strand::PointLight light = { postion, ambient, diffuse, specular };
		m_Lights.emplace_back(light);
		
		Strand::MaterialD matrial = { m_Shininess };

		SD_PROFILE_SCOPE("Renderer Draw");
		

		Strand::Renderer3D::BeginScene(m_EditorCamera, m_Lights);
		Strand::Renderer3D::DrawCubeMesh(model, m_CubeMesh, m_CubeColor, glm::vec4(m_EditorCamera.GetPosition(), 1.0f), matrial, m_Diffuse, m_Specular);
		Strand::Renderer3D::EndScene();

		m_EditorCamera.OnUpdate(ts);
		
	}

}

void Sandbox3D::OnImGuiRender()
{
	SD_PROFILE_FUNCTION();



	ImGui::Begin("Settings");

	auto stats = Strand::Renderer3D::GetStats();
	ImGui::Text("Renderer3D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	//ImGui::Text("Quads: %d", stats.QuadCount);
	//ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	//ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit3("m_CubeColor Color", glm::value_ptr(m_CubeColor));
	ImGui::ColorEdit4("m_Ambient Color", glm::value_ptr(m_Ambient));
	//ImGui::ColorEdit4("m_Diffuse Color", glm::value_ptr(m_Diffuse));
	//ImGui::ColorEdit4("m_Specular HighLight", glm::value_ptr(m_Specular));
	ImGui::DragFloat("m_Shininess", &m_Shininess);
	ImGui::End();

}




void Sandbox3D::OnEvent(Strand::Event& e)
{
	m_CameraController.OnEvent(e);
}