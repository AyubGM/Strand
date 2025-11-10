#pragma once

#include <vector>
#include <filesystem>

namespace Strand {

	class EditorSettings
	{
	public:
		static EditorSettings& Get();

		void Load();
		void Save();

		void AddRecentProject(const std::filesystem::path& path);
		const std::vector<std::filesystem::path>& GetRecentProjects() const { return m_RecentProjects; }

	private:
		EditorSettings() = default;
		std::filesystem::path GetConfigFilePath() const;

	private:
		std::vector<std::filesystem::path> m_RecentProjects;
		const size_t m_MaxRecentProjects = 10;
	};
}
