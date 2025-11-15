#include "sdpch.h"
#include "../EditorLayer.h"
#include "ScriptEditorPanel.h"
#include <ImGui/imgui.h>

#include <fstream>
#include <sstream>

namespace Strand {

	ScriptEditorPanel::ScriptEditorPanel(EditorLayer* editorLayer)
		: m_EditorLayer(editorLayer)
	{
		auto lang = GetCSharpLanguageDefinition();
		m_TextEditor.SetLanguageDefinition(lang);

	}

	void ScriptEditorPanel::OpenFile(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
		{
			SD_CORE_ERROR("Script file does not exist: {0}", path.string());
			return;
		}

		if (path.extension() != ".cs")
		{
			SD_CORE_WARN("Attempted to open non-script file in editor: {0}", path.string());
			return;
		}

		std::ifstream file(path);
		if (!file.is_open())
		{
			SD_CORE_ERROR("Failed to open script file: {0}", path.string());
			return;
		}
		std::stringstream ss;
		ss << file.rdbuf();
		file.close();

		// Set editor state
		m_CurrentFile = path;
		m_TextEditor.SetText(ss.str());
		//m_TextEditor.ClearUndoHistory();
		m_FileDirty = false;

		SD_CORE_TRACE("Opened script file: {0}", path.string());
	}

	void ScriptEditorPanel::SaveFile()
	{
		if (m_CurrentFile.empty())
			return;

		std::ofstream file(m_CurrentFile);
		if (!file.is_open())
		{
			SD_CORE_ERROR("Failed to save script file: {0}", m_CurrentFile.string());
			return;
		}

		file << m_TextEditor.GetText();
		file.close();
		m_FileDirty = false;

		if (m_EditorLayer)
		{
			m_EditorLayer->ReloadProjectScripts();
		}

		SD_CORE_TRACE("Saved script file: {0}", m_CurrentFile.string());

		//ScriptEngine::ReloadAssembly();
	}

	void ScriptEditorPanel::HandleShortcuts()
	{
		// Check if the editor window is focused
		if (!ImGui::IsWindowFocused())
			return;

		bool control = ImGui::GetIO().KeyCtrl;
		if (control && ImGui::IsKeyPressed(ImGuiKey_S, false)) // false = not repeating
		{
			SaveFile();
		}
	}

	void ScriptEditorPanel::OnImGuiRender()
	{
		// Don't render if no file is open
		if (m_CurrentFile.empty())
			return;

		// Create a dynamic window title
		std::string title = m_CurrentFile.filename().string();
		if (m_FileDirty)
			title += "*";

		// Use ### to give a stable ID, so ImGui docking works
		title += "###ScriptEditor";

		bool p_open = true; // Use a local variable for the window open/close state
		ImGui::Begin(title.c_str(), &p_open, ImGuiWindowFlags_MenuBar);

		// Handle window close
		if (!p_open)
		{
			// The user clicked the 'X' button
			if (m_FileDirty)
			{
				// TODO, you'd show a "Save changes?" popup here.
				// For now, we'll just log a warning and discard changes.
				SD_CORE_WARN("Script '{0}' closed with unsaved changes. Discarding.", m_CurrentFile.string());
			}

			// Clear the editor state
			m_CurrentFile = "";
			m_TextEditor.SetText("");
			m_FileDirty = false;
			ImGui::End();
			return;
		}

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Handle keyboard shortcuts
		HandleShortcuts();

		// Render the text editor
		m_TextEditor.Render("TextEditor");

		// Check for text changes after rendering to set the dirty flag
		if (m_TextEditor.IsTextChanged())
		{
			m_FileDirty = true;
		}

		ImGui::End();
	}



	TextEditor::LanguageDefinition ScriptEditorPanel::GetCSharpLanguageDefinition()
	{
		TextEditor::LanguageDefinition lang;
		lang.mName = "C#";

		// C# Keywords
		static const char* const keywords[] = {
			"abstract", "as", "base", "bool", "break", "byte", "case", "catch", "char", "checked", "class", "const", "continue",
			"decimal", "default", "delegate", "do", "double", "else", "enum", "event", "explicit", "extern", "false", "finally",
			"fixed", "float", "for", "foreach", "goto", "if", "implicit", "in", "int", "interface", "internal", "is", "lock",
			"long", "namespace", "new", "null", "object", "operator", "out", "override", "params", "private", "protected",
			"public", "readonly", "ref", "return", "sbyte", "sealed", "short", "sizeof", "stackalloc", "static", "string",
			"struct", "switch", "this", "throw", "true", "try", "typeof", "uint", "ulong", "unchecked", "unsafe", "ushort",
			"using", "virtual", "void", "volatile", "while", "get", "set", "partial", "yield", "where", "var"
		};

		for (auto& k : keywords)
			lang.mKeywords.insert(k);

		// Common Strand/Engine Identifiers (can add more)
		static const char* const identifiers[] = {
			"Entity", "Vector2", "Vector3", "Vector4", "TransformComponent", "TagComponent", "CameraComponent", "SpriteRendererComponent",
			"ScriptableEntity", "Log", "Input", "Key", "OnCreate", "OnDestroy", "OnUpdate", "GetComponent"
		};
		TextEditor::Identifier id;
		id.mDeclaration = "Built-in Type/Function";
		for (auto& i : identifiers)
			lang.mIdentifiers.insert(std::make_pair(std::string(i), id));

		// Comments
		lang.mCommentStart = "/*";
		lang.mCommentEnd = "*/";
		lang.mSingleLineComment = "//";

		// Preprocessor
		lang.mPreprocChar = '#';

		// Auto-indentation
		lang.mAutoIndentation = true;

		// Case sensitivity
		lang.mCaseSensitive = true;

		return lang;

	}

}