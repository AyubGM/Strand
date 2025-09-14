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


	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};