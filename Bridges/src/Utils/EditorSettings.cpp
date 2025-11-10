#include "sdpch.h"
#include "EditorSettings.h"

#include <fstream>

namespace Strand {

	EditorSettings& EditorSettings::Get()
	{
		static EditorSettings instance;
		return instance;
	}

	std::filesystem::path EditorSettings::GetConfigFilePath() const
	{
		//TODO A more robust solution might use %APPDATA% or a similar user-specific folder.
		return "BridgesEditor.config";
	}

	void EditorSettings::Load()
	{
		m_RecentProjects.clear();
		std::ifstream file(GetConfigFilePath());
		if (!file.is_open())
			return;

		std::string pathStr;
		while (std::getline(file, pathStr))
		{
			if (!pathStr.empty())
				m_RecentProjects.push_back(pathStr);
		}

	}

	void EditorSettings::Save()
	{
		std::ofstream file(GetConfigFilePath());
		if (!file.is_open())
			return;

		for (const auto& path : m_RecentProjects)
		{
			file << path.string() << "\n";
		}
	}

	void EditorSettings::AddRecentProject(const std::filesystem::path& path)
	{
		std::filesystem::path canonicalPath;
		try
		{
			// Get an absolute, clean path
			canonicalPath = std::filesystem::absolute(path);
		}
		catch (const std::filesystem::filesystem_error&)
		{
			// Fallback if path is invalid
			canonicalPath = path;
		}
		// Find if it already exists
		auto it = std::find(m_RecentProjects.begin(), m_RecentProjects.end(), canonicalPath);

		// If it exists, remove it from its old position
		if (it != m_RecentProjects.end())
		{
			m_RecentProjects.erase(it);
		}

		// Add it to the front (most recent)
		m_RecentProjects.insert(m_RecentProjects.begin(), canonicalPath);

		// Trim the list if it's too long
		if (m_RecentProjects.size() > m_MaxRecentProjects)
		{
			m_RecentProjects.pop_back();
		}

		// Save the updated list
		Save();


	}

}