#pragma once

#include "Asset.h"
#include "AssetMetadata.h"

#include "Strand/Scene/Components.h"

namespace Strand {

	class ScriptImporter
	{
	public:
		static Ref<ScriptComponent> ImportScript(AssetHandle handle, const AssetMetadata& metadata);
		static Ref<ScriptComponent> LoadScript(const std::filesystem::path& path);
	};
}