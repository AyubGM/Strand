#pragma once

#include "Strand/Core/UUID.h"

namespace Strand {

	using AssetHandle = UUID;

	enum class AssetType : uint16_t
	{
		None = 0,
		Scene,
		Texture2D,
		Texture3D,
		TextureCube,
		Script


	};

	std::string_view AssetTypeToString(AssetType type);
	AssetType AssetTypeFromString(std::string_view assetType);

	class Asset
	{
	public:
		AssetHandle Handle; // Generate handle

		virtual AssetType GetType() const = 0;
	};
}