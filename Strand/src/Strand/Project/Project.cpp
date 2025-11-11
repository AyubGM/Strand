#include "sdpch.h"
#include "Strand/Project/Project.h"

#include "Strand/Project/ProjectSerializer.h"

namespace Strand {

	std::filesystem::path Project::GetAssetAbsolutePath(const std::filesystem::path& path)
	{
		return GetAssetDirectory() / path;
	}

	Ref<Project> Project::New()
	{
		s_ActiveProject = CreateRef<Project>();

		return s_ActiveProject;
	}

	Ref<Project> Project::New(const std::filesystem::path& projectDirectory)
	{
		SD_CORE_ASSERT(!projectDirectory.empty(), "Project directory must not be empty");

		Ref<Project> project = CreateRef<Project>();

		if (project->m_Config.AssetDirectory.empty())
			project->m_Config.AssetDirectory = "Assets";

		// Ensure the project directory and asset directory exist on disk
		std::error_code ec;
		std::filesystem::create_directories(projectDirectory, ec);
		if (ec)
		{
			SD_CORE_ERROR("Failed to create project directory '{0}': {1}", projectDirectory.string(), ec.message());
			return nullptr;
		}
		// Create subfolders
		auto assetDir = projectDirectory / project->m_Config.AssetDirectory;
		auto scenesDir = projectDirectory / "Scenes";
		auto scriptsDir = projectDirectory / "Scripts";

		std::filesystem::create_directories(assetDir, ec);
		if (ec)
		{
			SD_CORE_ERROR("Failed to create asset directory '{0}': {1}", assetDir.string(), ec.message());
			return nullptr;
		}

		std::filesystem::create_directories(scenesDir, ec);
		std::filesystem::create_directories(scriptsDir, ec);

		// Default scene
		auto defaultScene = scenesDir / "Default.strand";
		if (!std::filesystem::exists(defaultScene))
		{
			std::ofstream sceneFile(defaultScene);
			sceneFile << "// Empty default scene\n";
			sceneFile.close();
		}

		// Starter C# script
		auto starterScript = scriptsDir / "Startup.cs";
		if (!std::filesystem::exists(starterScript))
		{
			std::ofstream scriptFile(starterScript);
			scriptFile <<
				"using System;\n"
				"namespace GameNamespace\n"
				"{\n"
				"    public class Startup\n"
				"    {\n"
				"        public static void Init()\n"
				"        {\n"
				"            Console.WriteLine(\"Hello from Startup script!\");\n"
				"        }\n"
				"    }\n"
				"}\n";
			scriptFile.close();
		}


		project->m_ProjectDirectory = projectDirectory;

		s_ActiveProject = project;

		// Initialize the EditorAssetManager and write an empty registry
		std::shared_ptr<EditorAssetManager> editorAssetManager = std::make_shared<EditorAssetManager>();
		s_ActiveProject->m_AssetManager = editorAssetManager;
		editorAssetManager->SerializeAssetRegistry(); // writes empty registry to AssetRegistry.yaml
		
		return s_ActiveProject;

	}

	Ref<Project> Project::Load(const std::filesystem::path& path)
	{
		Ref<Project> project = CreateRef<Project>();

		ProjectSerializer serializer(project);
		if (serializer.Deserialize(path))
		{
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;
			std::shared_ptr<EditorAssetManager> editorAssetManager = std::make_shared<EditorAssetManager>();
			s_ActiveProject->m_AssetManager = editorAssetManager;
			editorAssetManager->DeserializeAssetRegistry();
			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::SaveActive(const std::filesystem::path& path)
	{
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(path))
		{
			s_ActiveProject->m_ProjectDirectory = path.parent_path();
			return true;
		}

		return false;
	}

}