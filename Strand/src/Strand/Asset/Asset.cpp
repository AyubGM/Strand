#include "sdpch.h"
#include "Strand/Asset/Asset.h"

namespace Strand {

	std::string_view AssetTypeToString(AssetType type)
	{
		switch (type)
		{
		case AssetType::None:         return "AssetType::None";
		case AssetType::Scene:        return "AssetType::Scene";
		case AssetType::Texture2D:    return "AssetType::Texture2D";
		case AssetType::Texture3D:    return "AssetType::Texture3D";
		case AssetType::TextureCube:  return "AssetType::TextureCube";
		}

		return "AssetType::<Invalid>";
	}

	AssetType AssetTypeFromString(std::string_view assetType)
	{
		if (assetType == "AssetType::None")         return AssetType::None;
		if (assetType == "AssetType::Scene")        return AssetType::Scene;
		if (assetType == "AssetType::Texture2D")    return AssetType::Texture2D;
		if (assetType == "AssetType::Texture3D")    return AssetType::Texture3D;
		if (assetType == "AssetType::TextureCube")  return AssetType::TextureCube;

		return AssetType::None;
	}

}