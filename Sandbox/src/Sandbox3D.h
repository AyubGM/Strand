#pragma once


#include "Strand.h"

class Sandbox3D : public Strand::Layer
{
public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Strand::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Strand::Event& e) override;
private:
	Strand::OrthographicCameraController m_CameraController;
	std::vector<Strand::PointLight> m_Lights;
	Strand::EditorCamera m_EditorCamera;
	//Strand::Mesh m_CubeMesh;
	Strand::Ref<Strand::Mesh> m_CubeMesh;

	// Temp
	Strand::Ref<Strand::VertexArray> m_SquareVA;
	Strand::Ref<Strand::Shader> m_FlatColorShader;

	Strand::Ref<Strand::Texture2D> m_CheckerboardTexture;
	
	glm::vec3 m_CubeColor = glm::vec3(1.0f, 0.5f, 0.31f);

	glm::vec4 m_Ambient = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
	//glm::vec4 m_Diffuse = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
	Strand::Ref<Strand::Texture2D> m_Diffuse;
	//glm::vec4 m_Specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	Strand::Ref<Strand::Texture2D> m_Specular;
	float m_Shininess = 32.0f;
	Strand::Ref<Strand::Shader> m_PhongShader;
	Strand::Ref<Strand::Material> m_Material;


	//TEST
	Strand::Ref<Strand::Shader> m_PbrShader;
	Strand::Ref<Strand::Material> m_RustedIronMaterial;
	Strand::Ref<Strand::Mesh> m_SphereMesh;
	Strand::Ref<Strand::Texture2D> m_AlbedoTexture;
	//TEST

	
};