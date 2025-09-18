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
	Strand::EditorCamera m_EditorCamera;
	//Strand::Mesh m_CubeMesh;
	Strand::Ref<Strand::Mesh> m_CubeMesh;

	// Temp
	Strand::Ref<Strand::VertexArray> m_SquareVA;
	Strand::Ref<Strand::Shader> m_FlatColorShader;

	Strand::Ref<Strand::Texture2D> m_CheckerboardTexture;
	
	glm::vec3 m_CubeColor = glm::vec3(1.0f, 0.5f, 0.31f);

	glm::vec4 m_Ambient = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
	glm::vec4 m_Diffuse = glm::vec4(1.0f, 0.5f, 0.31f, 1.0f);
	glm::vec4 m_Specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	float m_Shininess = 32.0f;
	
};