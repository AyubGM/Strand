#include "sdpch.h"
#include "Strand/Project/Project.h"

#include "Strand/Project/ProjectSerializer.h"
#include "Strand/Scene/Scene.h"
#include "Strand/Scene/SceneSerializer.h"

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

	Ref<Project> Project::New(const std::filesystem::path& projectDirectory, const std::string& projectName)
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
		auto scenesDir = assetDir / "Scenes";
		auto scriptsDir = assetDir / "Scripts";
		auto binariesDir = scriptsDir / "Binaries";

		std::filesystem::create_directories(assetDir, ec);
		if (ec)
		{
			SD_CORE_ERROR("Failed to create asset directory '{0}': {1}", assetDir.string(), ec.message());
			return nullptr;
		}

		std::filesystem::create_directories(scenesDir, ec);
		std::filesystem::create_directories(scriptsDir, ec);
		std::filesystem::create_directories(binariesDir, ec);

		// Default scene
		auto defaultSceneDir = scenesDir / "Default.strand";
		if (!std::filesystem::exists(defaultSceneDir))
		{
			UUID cameraID;
			UUID squareID;
			UUID scriptEntityID;

			std::string sceneTemplate = LoadTemplate("Resources/Templates/DefaultScene.strand.template");
			std::unordered_map<std::string, std::string> values = 
			{
				{"SCENE_NAME", "Default"},
				{"CAMERA_ID", std::to_string(cameraID)},
				{"OBJECT_ID", std::to_string(squareID)},
				{"SCRIPT_ENTITY_ID", std::to_string(scriptEntityID)},
				{"NAMESPACE", SanitizeNamespace(projectName)}
			};

			std::string finalScene = ReplacePlaceholders(sceneTemplate, values);

			std::ofstream sceneFile(defaultSceneDir);
			sceneFile << finalScene;

		}

		// Starter C# script
		auto starterScript = scriptsDir / "Startup.cs";
		if (!std::filesystem::exists(starterScript))
		{
			std::string scriptTemplate = LoadTemplate("Resources/Templates/Startup.cs.template");
			std::unordered_map<std::string, std::string> values =
			{
				{"NAMESPACE", SanitizeNamespace(projectName)},
			//	{"CLASSNAME", "Startup"},
			//	{"PROJECT_NAME", projectName}
			};

			scriptTemplate = ReplacePlaceholders(scriptTemplate, values);

			std::ofstream scriptFile(starterScript);
			scriptFile << scriptTemplate;
		}

	/*	std::string_view namespaceName = projectName;
		if (!std::filesystem::exists(starterScript))
		{
			std::ofstream scriptFile(starterScript);
			scriptFile <<
				"using Strand;\n"
				"using System;\n"
				"namespace " << namespaceName << "\n"
				"{\n"
				"    public class Startup : Entity\n"
				"    {\n"
				"        public static void Init()\n"
				"        {\n"
				"            Console.WriteLine(\"Hello from Startup script!\");\n"
				"        }\n"
				"    public void OnCreate()\n"
				"        {\n"
				"            Console.WriteLine(\"Hello from Startup script!\");\n"
				"        }\n"
				"    }\n"
				"}\n";
			scriptFile.close();
		}*/

		Ref<Scene> defualtScene = CreateRef<Scene>();
		SceneSerializer serializer(defualtScene);
		serializer.Deserialize(defaultSceneDir);


		project->GetConfig().Name = projectName;
		project->GetConfig().StartScene = defualtScene->Handle;
		project->m_ProjectDirectory = projectDirectory;
		auto engineDllSource = std::filesystem::path("Resources/Scripts/Strand-ScriptCore.dll");
		auto enginePdbSource = std::filesystem::path("Resources/Scripts/Strand-ScriptCore.pdb");
		std::filesystem::copy_file(engineDllSource, binariesDir / "Strand-ScriptCore.dll" , std::filesystem::copy_options::overwrite_existing);
		std::filesystem::copy_file(enginePdbSource, binariesDir / "Strand-ScriptCore.pdb" , std::filesystem::copy_options::overwrite_existing);
		project->GetConfig().ScriptModulePath = binariesDir / (project->GetConfig().Name + ".dll");


		s_ActiveProject = project;

		// Initialize the EditorAssetManager and write an empty registry
		std::shared_ptr<EditorAssetManager> editorAssetManager = std::make_shared<EditorAssetManager>();
		s_ActiveProject->m_AssetManager = editorAssetManager;
		editorAssetManager->AddAsset(defualtScene, std::filesystem::relative(defaultSceneDir , assetDir));
		editorAssetManager->SerializeAssetRegistry(); // writes registry to AssetRegistry.yaml
		
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

	std::string Project::LoadTemplate(const std::filesystem::path& path)
	{
		std::ifstream file(path);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	std::string Project::ReplacePlaceholders(std::string text, const std::unordered_map<std::string, std::string>& values)
	{
		for (const auto& [key, value] : values)
		{
			std::string placeholder = "{{" + key + "}}";
			size_t pos = 0;
			while ((pos = text.find(placeholder, pos)) != std::string::npos)
			{
				text.replace(pos, placeholder.length(), value);
				pos += value.length();
			}
		}
		
		return text;
	}

	std::string Project::SanitizeNamespace(const std::string& name) {
		std::string ns = name;
		for (auto& c : ns) {
			if (!std::isalnum(c)) c = '_';
		}
		return ns;
	}


}