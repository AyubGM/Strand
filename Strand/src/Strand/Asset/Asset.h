#pragma once

#include "Strand/Core/UUID.h"

namespace Strand {

	using AssetHandle = UUID;

	enum class AssetType : uint16_t
	{
		None = 0,
		Scene,
		Texture2D
	};

	class Asset
	{
	public:
		AssetHandle Handle; // Generate handle

		virtual AssetType GetType() const = 0;
	};
}