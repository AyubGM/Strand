#pragma once

namespace Strand {

	class EditorLayer;

	class GetStartedPanel
	{
	public:
		GetStartedPanel(EditorLayer* editorLayer);
		void OnImGuiRender();

	private:
		EditorLayer* m_EditorLayer;
	};

}