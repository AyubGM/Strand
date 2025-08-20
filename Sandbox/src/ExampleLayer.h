#pragma once

#include "Strand.h"

class ExampleLayer : public Strand::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Strand::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Strand::Event& e) override;
private:
	Strand::ShaderLibrary m_ShaderLibrary;
	Strand::Ref<Strand::Shader> m_Shader;
	Strand::Ref<Strand::VertexArray> m_VertexArray;

	Strand::Ref<Strand::Shader> m_FlatColorShader;
	Strand::Ref<Strand::VertexArray> m_SquareVA;

	Strand::Ref<Strand::Texture2D> m_Texture, m_ChernoLogoTexture;

	Strand::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};