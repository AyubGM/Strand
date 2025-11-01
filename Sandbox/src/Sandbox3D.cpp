#include "Sandbox3D.h"

#include "Strand/Asset/TextureImporter.h"

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

std::vector< glm::vec4> pointLightPositions = {
	  glm::vec4(0.7f,  0.2f,  2.0f, 1.0f),
	  glm::vec4(2.3f, -3.3f, -4.0f, 1.0f),
	  glm::vec4(-4.0f,  2.0f, -12.0f, 1.0f),
	  glm::vec4(0.0f,  0.0f, -3.0f, 1.0f)
};
std::vector<std::string> paths
{
	    "assets/textures/skybox/right.jpg",
		"assets/textures/skybox/left.jpg",
		"assets/textures/skybox/top.jpg",
		"assets/textures/skybox/bottom.jpg",
		"assets/textures/skybox/front.jpg",
		"assets/textures/skybox/back.jpg"
};

void ProcessCollision(Strand::Manifold& manifold, float ts)
{
	const float totalSeparation = manifold.Depth + 0.01f;


	float separationA = 0.0f; // Amount to move A
	float separationB = 0.0f; // Amount to move B

	if (manifold.ObjA->IsStatic && manifold.ObjB->IsStatic) {
		std::cout << " Both objects are static. No movement applied.\n";
		return;
	}
	else if (manifold.ObjA->IsStatic) {
		// A is Static, B must move the full amount (opposite normal)
		separationB = totalSeparation;
		std::cout << " A is Static, B moves the full distance.\n";
	}
	else if (manifold.ObjB->IsStatic) {
		// B is Static, A must move the full amount (along normal)
		separationA = totalSeparation ;
		std::cout << "  B is Static, A moves the full distance.\n";
	}
	else {
		// Both Dynamic: Share the separation equally (inverse mass weighting is more accurate)
		separationA = totalSeparation * 0.5f;
		separationB = totalSeparation * 0.5f;
		std::cout << " Both Dynamic, separation split equally.\n";
	}

	// Apply movement to ObjA (in the direction of the Normal)
	manifold.ObjA->Transform.Translation += manifold.Normal * separationA;

	// Apply movement to ObjB (opposite the direction of the Normal)
	manifold.ObjB->Transform.Translation += ts * manifold.Normal * -separationB;

	std::cout << "  Separation Applied. A moved by " << separationA
		<< ", B moved by " << separationB << ".\n";

	// You would typically put logic here to:
	// 1. Resolve the collision (e.g., separate the objects).
	// 2. Reduce health, play a sound, etc.
}

Sandbox3D::Sandbox3D()
	: Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f), m_EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f), m_PlaneCol()
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

	m_SkyBoxMesh = Strand::CreateRef<Strand::Mesh>(cubeVertices, cubeindces);

	m_PhysicsWorld.SetGravity(glm::vec3(0, -9.81f, 0));
	m_PhysicsWorld.SetCollisionCallback(&ProcessCollision);
	
}

void Sandbox3D::OnAttach()
{
	SD_PROFILE_FUNCTION();

	Strand::Renderer3D::Init();

	m_Diffuse = Strand::TextureImporter::LoadTexture2D("assets/textures/container2.png");
	m_Specular = Strand::TextureImporter::LoadTexture2D("assets/textures/container2_specular.png");
	m_PhongShader = Strand::Shader::Create("assets/shaders/Renderer3D_Defualt.glsl");
	m_Material = Strand::Material::Create(m_PhongShader);
	m_Material->Set("u_MaterialData", m_Shininess);
	m_Material->Set("u_DiffuseTexture", m_Diffuse);
	m_Material->Set("u_SpecularTexture", m_Specular);

	m_LightShader = Strand::Shader::Create("assets/shaders/Renderer3D_LightCube.glsl");
	m_LightMaterial = Strand::Material::Create(m_LightShader);

	m_InstacedShader = Strand::Shader::Create("assets/shaders/Renderer3D_Instanced.glsl");
	m_InstacedMaterial = Strand::Material::Create(m_InstacedShader);
	m_InstacedMaterial->Set("u_DiffuseTexture", m_Diffuse);

	//m_ModelShader = Strand::Shader::Create("assets/shaders/Renderer3D_Model.glsl");
	//m_Backpack = Strand::CreateRef<Strand::Model>("assets/models/backpack/backpack.obj", m_ModelShader);
	


	m_CubeMapShader = Strand::Shader::Create("assets/shaders/Renderer3D_CubeMap.glsl");
	m_CubeMapTextur = Strand::TextureCube::Create(paths);

	m_ReflectShader = Strand::Shader::Create("assets/shaders/Renderer3D_Reflection.glsl");
	m_ReflectiveMaterial = Strand::Material::Create(m_ReflectShader);
	m_ReflectiveMaterial->Set("skybox", m_CubeMapTextur);

	Strand::HullCollider<Strand::Dimension::D3> temp = Strand::MakeCubeCollider();

	m_TestCollider.Points = temp.Points;
	Strand::Collider* cubeCollider = &m_TestCollider;
	m_TransformComponent.Translation = glm::vec3(0, 100, 0);
	m_TransformComponent.Scale = glm::vec3(1, 1, 1);
	m_TransformComponent.Rotation = glm::vec3(0, 15, 0);
	m_Rigidbody.SetTransform(&m_TransformComponent);
	m_Rigidbody.Collider = cubeCollider;
	m_Rigidbody.IsStatic = false;
	m_Rigidbody.IsSimulated = true;
	m_Rigidbody.Restitution = .5;
	m_Rigidbody.DynamicFriction = .2;
	m_Rigidbody.StaticFriction = .3;
	m_Rigidbody.SetMass(10.0);

	m_TransformComponent.Translation = glm::vec3(0, 0, 0);
	m_Plane.SetTransform(&m_TransformComponent);
	//Strand::Collider* planeCollider = &m_PlaneCol;
	m_Plane.Collider = &m_PlaneCol;
	//m_Rigidbody.Velocity = glm::vec3(1, 1, 1);
	m_PhysicsWorld.AddRigidbody(&m_Rigidbody);
	m_PhysicsWorld.AddCollisionObject(&m_Plane);
}

void Sandbox3D::OnDetach()
{
	SD_PROFILE_FUNCTION();
	Strand::Renderer3D::Shutdown();
}

void Sandbox3D::OnUpdate(Strand::Timestep ts)
{
	SD_PROFILE_FUNCTION();

	{
		m_DeltaTime = ts;
		m_Accumlator += ts;
		m_Frames++;
		if (m_Accumlator >= 1.0f)
		{
			m_FPS = m_Frames / m_Accumlator;
			m_Frames = 0;
			m_Accumlator = 0.0f;
		}
	}

	// Update
	m_CameraController.OnUpdate(ts);

	// Physics
	m_PhysicsWorld.Step(ts);


	// Render
	Strand::Renderer3D::ResetStats();

	{
		SD_PROFILE_SCOPE("Renderer Prep");
		Strand::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Strand::RenderCommand::Clear();
	}

	{
		m_Lights.clear();
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		glm::vec4 pointPostion = glm::vec4(1.2f, 1.0f, 2.0f, 1.0f);
		glm::vec4 pointAmbient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		glm::vec4 pointDiffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		glm::vec4 pointSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float pointConstant = 1.0f;
		float pointLinear = 0.09f;
		float pointQuadratic = 0.032f;
		
		for (uint32_t i = 0; i < pointLightPositions.size(); ++i)
		{
			Strand::PointLight light = { pointLightPositions[i], pointAmbient, pointDiffuse, pointSpecular, pointConstant, pointLinear, pointQuadratic };
			m_Lights.emplace_back(light);
		}

		glm::vec4 directLightDirection = glm::vec4(-0.2f, -1.0f, -0.3f, 1.0f);
		glm::vec4 directLightAmbient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
		glm::vec4 directLightDiffuse = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
		glm::vec4 directLightSpecular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

		Strand::DirectLight directLight = { directLightDirection, directLightAmbient, directLightDiffuse, directLightSpecular };

		glm::vec4 spotLightPostion = glm::vec4(m_EditorCamera.GetPosition(), 1.0f);
		glm::vec4 spotLightDirection = glm::vec4(m_EditorCamera.GetForwardDirection(), 1.0f);
		glm::vec4 spotLightAmbient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		glm::vec4 spotLightDiffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		glm::vec4 spotLightSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float spotLightConstant = 1.0f;
		float spotLightLinear = 0.09f;
		float spotLightQuadratic = 0.032f;
		float spotLightCutOff = glm::cos(glm::radians(12.5f));
		float spotLightOuterCutOff = glm::cos(glm::radians(15.0f));

		Strand::Spotlight spotLight = {
		spotLightPostion, spotLightDirection, spotLightAmbient, spotLightDiffuse,
		spotLightSpecular, spotLightCutOff, spotLightOuterCutOff, spotLightConstant,
		spotLightLinear, spotLightQuadratic
		};

		SD_PROFILE_SCOPE("Renderer Draw");

		//Strand::Renderer3D::BeginScene(m_EditorCamera, m_Lights, directLight, spotLight);

		std::array<Strand::PointLight, 4> lightsArray;
		std::copy_n(m_Lights.begin(), 4, lightsArray.begin());
		Strand::SceneData sceneData{ m_EditorCamera.GetPosition(), 4, lightsArray, directLight, spotLight };

		Strand::Renderer3D::BeginScene(m_EditorCamera, sceneData);

		Strand::Renderer3D::Submit(m_CubeMesh, m_ReflectiveMaterial, m_Rigidbody.Transform.GetTransform());


		glm::mat4 model = glm::mat4(1.0f);

		std::vector<glm::mat4> models;
		models.reserve(10);
		for (uint32_t i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			models.emplace_back(model);
			//Strand::Renderer3D::DrawCubeMesh(model, m_CubeMesh, m_CubeColor, m_Material);
		}
		Strand::Renderer3D::SubmitInstanced(m_CubeMesh, m_InstacedMaterial, models);
		
		//Strand::Renderer3D::DrawCubeMesh(glm::mat4(1), m_CubeMesh, m_CubeColor, m_ReflectiveMaterial);
		Strand::Renderer3D::Submit(m_CubeMesh, m_ReflectiveMaterial, glm::mat4(1));

		/*for (const auto& mesh : m_Backpack->GetMeshes())
		{
			uint32_t materialIndex = mesh->GetMaterialIndex();
			Strand::Ref<Strand::Material> material = m_Backpack->GetMaterials()[materialIndex];

			
			Strand::Renderer3D::DrawMesh(glm::mat4(1), mesh, material);
			Strand::Renderer3D::Submit(mesh, material, model);
		}*/


		for (uint32_t i = 0; i < pointLightPositions.size(); ++i)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(pointLightPositions[i]));
			model = glm::scale(model, glm::vec3(0.2f));
			//Strand::Renderer3D::DrawLightCube(model, m_CubeMesh, m_CubeColor);
			Strand::Renderer3D::Submit(m_CubeMesh, m_LightMaterial, model);
		}

		Strand::Renderer3D::BeginSkyboxPass(m_CubeMapTextur);

		//Strand::Renderer3D::DrawCubeMap( m_SkyBoxMesh, m_CubeMapTextur, m_CubeMapShader);
		Strand::Renderer3D::SubmitSkybox();

		Strand::Renderer3D::EndSkyboxPass();


		Strand::Renderer3D::EndScene();

		m_EditorCamera.OnUpdate(ts);
		
	}

}

void Sandbox3D::OnImGuiRender()
{
	SD_PROFILE_FUNCTION();


	ImGui::Begin("Stats");
	ImGui::Text("Delta Time: %.3f ms", m_DeltaTime * 1000.0f);
	ImGui::Text("FPS: %.1f ms", m_FPS);
	ImGui::End();

	ImGui::Begin("Settings");

	auto stats = Strand::Renderer3D::GetStats();
	ImGui::Text("Renderer3D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Mesh Count: %d", stats.GetTotalMeshCount());
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