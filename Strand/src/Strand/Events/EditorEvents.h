#pragma once

#include "Strand/Events/Event.h"
#include <filesystem>
#include <sstream>

namespace Strand {

	// Dispatched when a project path is selected from the hub
	class ProjectOpenedEvent : public Event
	{
	public:
		ProjectOpenedEvent(const std::filesystem::path& path) : m_Path(path) {}

		const std::filesystem::path& GetPath() const { return m_Path; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ProjectOpenedEvent: " << m_Path.string();
			return ss.str();
		}

		EVENT_CLASS_TYPE(ProjectOpened)
			EVENT_CLASS_CATEGORY(EventCategoryEditor)
	private:
		std::filesystem::path m_Path;
	};

	// Dispatched when "New Project" is clicked in the hub
	class NewProjectRequestEvent : public Event
	{
	public:
		NewProjectRequestEvent() = default;
		EVENT_CLASS_TYPE(NewProjectRequest)
			EVENT_CLASS_CATEGORY(EventCategoryEditor)
	};

	// Dispatched by EditorLayer when a project is successfully loaded/created
	class ProjectLoadSuccessEvent : public Event
	{
	public:
		ProjectLoadSuccessEvent() = default;
		EVENT_CLASS_TYPE(ProjectLoadSuccess)
			EVENT_CLASS_CATEGORY(EventCategoryEditor)
	};

	// Dispatched by ContentBrowser when a .cs file is double-clicked
	class ScriptFileOpenedEvent : public Event
	{
	public:
		ScriptFileOpenedEvent(const std::filesystem::path& path) : m_Path(path) {}

		const std::filesystem::path& GetPath() const { return m_Path; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScriptFileOpenedEvent: " << m_Path.string();
			return ss.str();
		}

		EVENT_CLASS_TYPE(ScriptFileOpened)
			EVENT_CLASS_CATEGORY(EventCategoryEditor)
	private:
		std::filesystem::path m_Path;
	};

	// Dispatched by ScriptEditorPanel when a file is saved
	class ScriptFileSavedEvent : public Event
	{
	public:
		ScriptFileSavedEvent(const std::filesystem::path& path) : m_Path(path) {}

		const std::filesystem::path& GetPath() const { return m_Path; }

		EVENT_CLASS_TYPE(ScriptFileSaved)
			EVENT_CLASS_CATEGORY(EventCategoryEditor)
	private:
		std::filesystem::path m_Path;
	};
}