#pragma once

#include "AssetManagerBase.h"
#include "AssetMetadata.h"

#include <map>

namespace Strand {

	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;

	class RuntimeAssetManager : public AssetManagerBase
	{
	public:
		virtual Ref<Asset> GetAsset(AssetHandle handle) override;
		virtual bool IsAssetHandleValid(AssetHandle handle) const override;
		virtual bool IsAssetLoaded(AssetHandle handle) const override;
		virtual AssetType GetAssetType(AssetHandle handle) const override;

		bool LoadAssetRegistry();

	private:
		AssetRegistry m_AssetRegistry; // Asset metadata (Handle -> Path/Type)
		AssetMap m_LoadedAssets;	   // Assets currently loaded in memory

		const AssetMetadata& GetMetadata(AssetHandle handle) const;
	};
}