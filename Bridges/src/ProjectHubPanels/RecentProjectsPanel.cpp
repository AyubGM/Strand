#include "sdpch.h"
#include "RecentProjectsPanel.h"
#include "../EditorLayer.h"
#include "../Utils/EditorSettings.h"
#include <imgui/imgui.h>

namespace Strand {

	RecentProjectsPanel::RecentProjectsPanel(EditorLayer* editorlayer) : m_EditorLayer(editorlayer)
	{
	}

	void RecentProjectsPanel::OnImGuiRender()
	{
		// Calculate panel width
		float leftPanelWidth = ImGui::GetContentRegionAvail().x * 0.6f - ImGui::GetStyle().ItemSpacing.x * 0.5f;

		ImGui::BeginChild("RecentProjectsPanel", ImVec2(leftPanelWidth, -1), true);
		{
			ImGui::Text("Recent Projects");
			ImGui::Separator();

			const auto& projects = EditorSettings::Get().GetRecentProjects();

			if (projects.empty())
			{
				ImGui::Text("No recent projects...");
			}
			else
			{
				for (const auto& path : projects)
				{
					// Use the project's filename as the selectable label
					if (ImGui::Selectable(path.filename().string().c_str()))
					{
						// Check if file still exists before trying to open
						if (std::filesystem::exists(path))
						{
							m_EditorLayer->OpenProject(path);
						}
						else
						{
							SD_CORE_WARN("Recent project file not found: {0}", path.string());
							// We could also remove it from the list here
							// EditorSettings::Get().RemoveRecentProject(path);
						}
					}

					// Show the full path on hover
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("%s", path.string().c_str());
				}
			}
		}
		ImGui::EndChild(); // End RecentProjectsPanel
	}
}