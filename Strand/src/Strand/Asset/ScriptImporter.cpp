#include "sdpch.h"
#include "ScriptImporter.h"

#include "Strand/Project/Project.h"

namespace Strand {

	Ref<ScriptClass> ScriptImporter::ImportScript(AssetHandle handle, const AssetMetadata& metadata)
	{
		SD_PROFILE_FUNCTION();

		return LoadScript(Project::GetActiveAssetDirectory() / metadata.FilePath);
	}

	Ref<ScriptClass> ScriptImporter::LoadScript(const std::filesystem::path& path)
	{
		SD_PROFILE_FUNCTION();

		// Load assembly into ScriptEngine
		if (!ScriptEngine::LoadAppAssembly(path))
		{
			SD_CORE_ERROR("Failed to load script assembly: {0}", path.string());
			return nullptr;
		}


		Ref<ScriptClass> script = CreateRef<ScriptClass>();

		return script;
	}


}
