#include "GameLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Strand;

GameLayer::GameLayer(): Layer("GameLayer")
{
	auto& window = Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());
	m_Camera->SetPosition({ 0.0f, 0.0f, 0.0f }); //TODO
	Random::Init();
}

void GameLayer::OnAttach()
{
	m_Level.Init();

	ImGuiIO io = ImGui::GetIO();
	m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 16.0f);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Strand::Timestep ts)
{
	m_Time += ts;
	if ((int)(m_Time * 10.0f) % 8 > 4)
		m_Blink = !m_Blink;

	if (m_Level.IsGameOver())
		m_State = GameState::GameOver;

	const auto& playerPos = glm::vec2(0.5f, 0.2f);//m_Level.GetPlayer().GetPosition();
	//m_Camera->SetPosition({ playerPos.x, playerPos.y, 0.0f });

	switch (m_State)
	{
	case GameState::Play:
	{
		m_Level.OnUpdate(ts);
		break;
	}
	}

	// Render
	Strand::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
	Strand::RenderCommand::Clear();

	Strand::Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRender();
	Strand::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	switch (m_State)
	{
	case GameLayer::GameState::Play:
	{
		uint32_t playerScore = 100; // TODO
		std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
		break;
	}
	case GameLayer::GameState::MainMenu:
	{ 
		auto pos = ImGui::GetWindowPos();
		auto width = Application::Get().GetWindow().GetWidth();
		auto hight = Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 300.0f;
		pos.y += 50.0f;
		if (m_Blink)
			ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
		break; 
	}
		
	case GameLayer::GameState::GameOver:
	{
		auto pos = ImGui::GetWindowPos();
		auto width = Application::Get().GetWindow().GetWidth();
		auto hight = Application::Get().GetWindow().GetHeight();
		pos.x += width * 0.5f - 300.0f;
		pos.y += 50.0f;
		if (m_Blink)
			ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");

		pos.x += 200.0f;
		pos.y += 150.0f;
		uint32_t playerScore = 100; //m_Level.GetPlayer().GetScore();
		std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
		break; 
	}
		
	case GameLayer::GameState::Pause:
	{
		//TODO
		break; 
	}
		
    }

}

void GameLayer::OnEvent(Strand::Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(SD_BIND_EVENT_FN(GameLayer::OnWindowResize));
	dispatcher.Dispatch<MouseButtonPressedEvent>(SD_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));

}


bool GameLayer::OnMouseButtonPressed(Strand::MouseButtonPressedEvent& e)
{
	if (m_State == GameState::GameOver)
	{
		m_Level.Reset();
	}

	m_State = GameState::Play;
	return false;
}

bool GameLayer::OnWindowResize(Strand::WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeight());
	return false;
}


void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
	float aspectRatio = (float)width / (float)height;

	float camWidth = 40.0f;
	float bottom = -camWidth;
	float top = camWidth;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	m_Camera = CreateScope<OrthographicCamera>(left, right, bottom, top);
}

