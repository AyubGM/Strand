#pragma once

#include <string>
#include <filesystem>

#include "Strand/Core/Base.h"

#include "Strand/Asset/RuntimeAssetManager.h"
#include "Strand/Asset/EditorAssetManager.h"

namespace Strand {

	struct ProjectConfig
	{
		std::string Name = "Untitled";

		AssetHandle StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path AssetRegistryPath = "AssetRegistry.yaml"; // Relative to AssetDirectory
		std::filesystem::path ScriptModulePath;
	};

	class Project
	{
	public:
		const std::filesystem::path& GetProjectDirectory() { return m_ProjectDirectory; }
		std::filesystem::path GetAssetDirectory() { return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory; }
		std::filesystem::path GetAssetRegistryPath() { return GetAssetDirectory() / s_ActiveProject->m_Config.AssetRegistryPath; }
		// TODO: move to asset manager when we have one
		std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path) { return GetAssetDirectory() / path; }

		std::filesystem::path GetAssetAbsolutePath(const std::filesystem::path& path);

		static const std::filesystem::path& GetActiveProjectDirectory()
		{
			SD_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetProjectDirectory();
		}

		static std::filesystem::path GetActiveAssetDirectory()
		{
			SD_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetAssetDirectory();
		}

		static std::filesystem::path GetActiveAssetRegistryPath()
		{
			SD_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetAssetRegistryPath();
		}

		// TODO: move to asset manager when we have one
		static std::filesystem::path GetActiveAssetFileSystemPath(const std::filesystem::path& path)
		{
			SD_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->GetAssetFileSystemPath(path);
		}


		ProjectConfig& GetConfig() { return m_Config; }

		static Ref<Project> GetActive() { return s_ActiveProject; }
		std::shared_ptr<AssetManagerBase> GetAssetManager() { return m_AssetManager; }
		std::shared_ptr<RuntimeAssetManager> GetRuntimeAssetManager() { return std::static_pointer_cast<RuntimeAssetManager>(m_AssetManager); }
		std::shared_ptr<EditorAssetManager> GetEditorAssetManager() { return std::static_pointer_cast<EditorAssetManager>(m_AssetManager); }

		static Ref<Project> New();
		static Ref<Project> New(const std::filesystem::path& projectDirectory, const std::string& projectName);
		static Ref<Project> Load(const std::filesystem::path& path);
		static void CreateScriptFile(const std::filesystem::path& path, const std::string& fileName);
		static void CreateSceneFile(const std::filesystem::path& path, const std::string& fileName);
		static bool SaveActive(const std::filesystem::path& path);

	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;
		std::shared_ptr<AssetManagerBase> m_AssetManager;

		inline static Ref<Project> s_ActiveProject;
	};

}