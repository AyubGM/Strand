#pragma once

#include "Strand.h"
#include "EditorLayer.h"
#include "ProjectHubPanels/GetStartedPanel.h"
#include "ProjectHubPanels/RecentProjectsPanel.h"

namespace Strand {

	class ProjectHubLayer : public Layer
	{
	public:
		ProjectHubLayer(EditorLayer* editorLayer);
		virtual ~ProjectHubLayer() = default;

		virtual void OnImGuiRender() override;
	private:
		EditorLayer* m_EditorLayer;

		Scope<RecentProjectsPanel> m_RecentProjectsPanel;
		Scope<GetStartedPanel> m_GetStartedPanel;

	};
}