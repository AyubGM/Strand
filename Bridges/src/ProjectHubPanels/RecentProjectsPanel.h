#pragma once

namespace Strand {

	class EditorLayer; // Forward declaration

	class RecentProjectsPanel
	{
	public:
		RecentProjectsPanel(EditorLayer* editorLayer);
		void OnImGuiRender();

	private:
		EditorLayer* m_EditorLayer;
	};

}