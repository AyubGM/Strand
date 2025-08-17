#pragma once

#include "Strand.h"

#include <imgui/imgui.h>

class GameLayer : public Strand::Layer
{
public:
	GameLayer ();
	virtual ~GameLayer () = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Strand::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Strand::Event& e) override;
	bool OnMouseButtonPressedEvent(Strand::MouseButtonPressedEvent& e);
	bool OnWindowResizeEvent(Strand::WindowResizeEvent& e);

private:
	void CreateCamera(uint32_t width, uint32_t height);
private:
	Strand::Scope<Strand::OrthographicCamera> m_Camera;
	ImFont* m_Font;
	float m_Time = 0.0f;
	bool m_Blink = false;

	enum class GameState
	{
		Play = 0, MainMenu = 1, GameOver = 2, Pause = 3
	};
	GameState m_State = GameState::MainMenu;
};

