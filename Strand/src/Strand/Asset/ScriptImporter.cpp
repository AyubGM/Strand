#include "sdpch.h"
#include "ScriptImporter.h"

#include "Strand/Project/Project.h"

namespace Strand {

	Ref<ScriptComponent> ScriptImporter::ImportScript(AssetHandle handle, const AssetMetadata& metadata)
	{
		SD_PROFILE_FUNCTION();

		return LoadScript(Project::GetActiveAssetDirectory() / metadata.FilePath);
	}

	Ref<ScriptComponent> ScriptImporter::LoadScript(const std::filesystem::path& path)
	{
		SD_PROFILE_FUNCTION();
	}


}
