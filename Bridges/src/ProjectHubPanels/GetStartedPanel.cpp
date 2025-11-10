#include "sdpch.h"
#include "GetStartedPanel.h"
#include "../EditorLayer.h"
#include <imgui/imgui.h>

namespace Strand {

	GetStartedPanel::GetStartedPanel(EditorLayer* editorLayer)
		: m_EditorLayer(editorLayer)
	{
	}

	void GetStartedPanel::OnImGuiRender()
	{
		ImGui::BeginChild("GetStartedPanel", ImVec2(-1, -1), true);
		{
			ImGui::Text("Get Started");
			ImGui::Separator();

			ImVec2 buttonSize(ImVec2(-1, 50)); // Full width, 50px height

			// "Create New Project" Button
			if (ImGui::Button("Create New Project", buttonSize))
			{
				// Call the EditorLayer's function
				m_EditorLayer->NewProject();
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

			// "Open Existing Project" Button
			if (ImGui::Button("Open Existing Project", buttonSize))
			{
				// Call the EditorLayer's function
				m_EditorLayer->OpenProject();
			}

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

			// Add a disabled button to mimic VS
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			ImGui::Button("Clone a Repository", buttonSize);
			ImGui::PopStyleVar();
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Not Implemented");

		}
		ImGui::EndChild(); // End GetStartedPanel
	}

}