#pragma once



#include "Strand.h"

class Sandbox2D : public Strand::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Strand::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Strand::Event& e) override;
private:
	Strand::OrthographicCameraController m_CameraController;

	// Temp
	Strand::Ref<Strand::VertexArray> m_SquareVA;
	Strand::Ref<Strand::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};