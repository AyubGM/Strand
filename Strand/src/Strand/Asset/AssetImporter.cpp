#include "sdpch.h"
#include "AssetImporter.h"

#include "TextureImporter.h"
#include "SceneImporter.h"

#include <map>

namespace Strand {

	using AssetImportFunction = std::function<Ref<Asset>(AssetHandle, const AssetMetadata&)>;
	static std::map<AssetType, AssetImportFunction> s_AssetImportFunctions = {
		{ AssetType::Scene,     SceneImporter::ImportScene },
		{ AssetType::Texture2D, TextureImporter::ImportTexture2D },
		//{ AssetType::Texture3D, TextureImporter::ImportTexture3D },
		//{ AssetType::TextureCube, TextureImporter::ImportTextureCube }
	};

	Ref<Asset> AssetImporter::ImportAsset(AssetHandle handle, const AssetMetadata& metadata)
	{
		if (s_AssetImportFunctions.find(metadata.Type) == s_AssetImportFunctions.end())
		{
			SD_CORE_ERROR("No importer available for asset type: {}", (uint16_t)metadata.Type);
			return nullptr;
		}

		return s_AssetImportFunctions.at(metadata.Type)(handle, metadata);
	}

}