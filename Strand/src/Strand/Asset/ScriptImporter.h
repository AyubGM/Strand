#pragma once

#include "Asset.h"
#include "AssetMetadata.h"

#include "Strand/Scripting/ScriptEngine.h"


namespace Strand {

	class ScriptImporter
	{
	public:
		static Ref<ScriptClass> ImportScript(AssetHandle handle, const AssetMetadata& metadata);
		static Ref<ScriptClass> LoadScript(const std::filesystem::path& path);
	};
}