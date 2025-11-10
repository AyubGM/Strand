#include "ProjectHubLayer.h"
#include <imgui/imgui.h>

namespace Strand {

	ProjectHubLayer::ProjectHubLayer(EditorLayer* editorLayer) : Layer("ProjectHubLayer"), m_EditorLayer(editorLayer)
	{
		m_RecentProjectsPanel = CreateScope<RecentProjectsPanel>(editorLayer);
		m_GetStartedPanel = CreateScope<GetStartedPanel>(editorLayer);
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

		// Push some padding for a more professional look
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
		ImGui::Begin("Project Hub", &p_open, window_flags);
		ImGui::PopStyleVar(); // Pop main window padding

		// Center the content
		auto contentRegionAvail = ImGui::GetContentRegionAvail();

		// Simple title
		const char* title = "Welcome to Bridges";
		ImGui::SetCursorPosX((contentRegionAvail.x - ImGui::CalcTextSize(title).x) * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + contentRegionAvail.y * 0.2f);
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Assuming Font[0] is larger/title font
		ImGui::TextUnformatted(title);
		ImGui::PopFont();


		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30); // Add more space after title

		// --- Main Layout Panels ---

		// Push style for child windows
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 15.0f));

		// --- Render Panels ---
		// The logic is now inside their own classes
		m_RecentProjectsPanel->OnImGuiRender();

		ImGui::SameLine();

		m_GetStartedPanel->OnImGuiRender();
		// --- End of Render Panels ---

		ImGui::PopStyleVar(2); // Pop child window styles

		ImGui::End();

	}

}