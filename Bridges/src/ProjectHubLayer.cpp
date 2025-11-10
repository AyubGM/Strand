#include "ProjectHubLayer.h"
#include <imgui/imgui.h>

namespace Strand {

	ProjectHubLayer::ProjectHubLayer(EditorLayer* editorLayer) : Layer("ProjectHubLayer"), m_EditorLayer(editorLayer)
	{
	}

	void ProjectHubLayer::OnImGuiRender()
	{
		if (m_EditorLayer->IsProjectOpen())
		{
			Application::Get().SubmitToMainThread([this]() {
				Application::Get().PopOverlay(this);
				});
			return;
		}

		static bool p_open = true; // Always true
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("Project Hub", &p_open, window_flags);

		// Center the content
		auto contentRegionAvail = ImGui::GetContentRegionAvail();

		// Simple title
		const char* title = "Welcome to Bridges";
		ImGui::SetCursorPosX((contentRegionAvail.x - ImGui::CalcTextSize(title).x) * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + contentRegionAvail.y * 0.2f);
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Assuming Font[0] is larger/title font
		ImGui::TextUnformatted(title);
		ImGui::PopFont();


		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

		// --- Buttons ---
		float buttonWidth = 200;
		float buttonHeight = 50;
		ImVec2 buttonSize(buttonWidth, buttonHeight);

		// "Create New Project" Button
		ImGui::SetCursorPosX((contentRegionAvail.x - buttonWidth) * 0.5f);
		if (ImGui::Button("Create New Project", buttonSize))
		{
			// Call the EditorLayer's function
			m_EditorLayer->NewProject();
		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

		// "Open Existing Project" Button
		ImGui::SetCursorPosX((contentRegionAvail.x - buttonWidth) * 0.5f);
		if (ImGui::Button("Open Existing Project", buttonSize))
		{
			// Call the EditorLayer's function
			m_EditorLayer->OpenProject();
		}

		ImGui::End();

	}

}