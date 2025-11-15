#pragma once

#include <filesystem>
#include <TextEditor.h>

namespace Strand {

	class EditorLayer;

	class ScriptEditorPanel
	{
	public:
		ScriptEditorPanel(EditorLayer* editorLayer);
		~ScriptEditorPanel() = default;

		void OnImGuiRender();
		void OpenFile(const std::filesystem::path& path);

	private:
		void SaveFile();
		void HandleShortcuts();
		static TextEditor::LanguageDefinition GetCSharpLanguageDefinition();

	private:
		TextEditor m_TextEditor;
		std::filesystem::path m_CurrentFile;
		bool m_FileDirty = false;
		EditorLayer* m_EditorLayer = nullptr; 


	};
}