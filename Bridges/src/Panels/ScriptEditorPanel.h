#pragma once

#include <filesystem>
#include <TextEditor.h>
#include "Strand/Events/Event.h"

namespace Strand {

	class EditorLayer;

	class ScriptEditorPanel
	{
	public:
		ScriptEditorPanel();
		~ScriptEditorPanel() = default;

		void OnAttach();
		void OnImGuiRender();
		void OpenFile(const std::filesystem::path& path);

	private:
		void OnScriptFileOpened(Event& e);
		void SaveFile();
		void HandleShortcuts();
		static TextEditor::LanguageDefinition GetCSharpLanguageDefinition();

	private:
		TextEditor m_TextEditor;
		std::filesystem::path m_CurrentFile;
		bool m_FileDirty = false;


	};
}