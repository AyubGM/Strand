#include "sdpch.h"
#include "RuntimeAssetManager.h"

#include "AssetImporter.h"
#include "Strand/Project/Project.h" // Needed to get AssetRegistryPath

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Strand {

	const AssetMetadata& RuntimeAssetManager::GetMetadata(AssetHandle handle) const
	{
		static AssetMetadata s_NullMetadata;
		auto it = m_AssetRegistry.find(handle);
		if (it == m_AssetRegistry.end())
			return s_NullMetadata;

		return it->second;
	}

	bool RuntimeAssetManager::IsAssetHandleValid(AssetHandle handle) const
	{
		// Asset handle is valid if it's non-zero AND we have metadata for it.
		return handle != 0 && m_AssetRegistry.find(handle) != m_AssetRegistry.end();
	}

	bool RuntimeAssetManager::IsAssetLoaded(AssetHandle handle) const
	{
		return m_LoadedAssets.find(handle) != m_LoadedAssets.end();
	}

	AssetType RuntimeAssetManager::GetAssetType(AssetHandle handle) const
	{
		if (!IsAssetHandleValid(handle))
			return AssetType::None;

		return m_AssetRegistry.at(handle).Type;
	}

	Ref<Asset> RuntimeAssetManager::GetAsset(AssetHandle handle)
	{
		if (!IsAssetHandleValid(handle))
		{
			// Log an error if an invalid handle is requested in runtime
			SD_CORE_ERROR("RuntimeAssetManager: Attempted to get invalid asset handle {0}", (uint64_t)handle);
			return nullptr;
		}

		// 1. Check if asset is already loaded
		if (IsAssetLoaded(handle))
		{
			return m_LoadedAssets.at(handle);
		}

		// 2. Asset is not loaded, look up metadata and import it
		const AssetMetadata& metadata = GetMetadata(handle);
		Ref<Asset> asset = AssetImporter::ImportAsset(handle, metadata);

		if (!asset)
		{
			SD_CORE_ERROR("RuntimeAssetManager: Asset import failed for handle {0} ('{1}')",
				(uint64_t)handle, metadata.FilePath.string());
			return nullptr;
		}

		// 3. Store the newly loaded asset and return it
		asset->Handle = handle;
		m_LoadedAssets[handle] = asset;

		return asset;
	}

	bool RuntimeAssetManager::LoadAssetRegistry()
	{
		// Get the path to the asset registry file (created by the Editor)
		auto path = Project::GetActiveAssetRegistryPath();

		if (!std::filesystem::exists(path))
		{
			// The registry file MUST exist in a runtime build; log an error if it doesn't.
			SD_CORE_ERROR("Asset Registry file not found at '{0}'. Runtime cannot proceed.", path.string());
			return false;
		}

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			SD_CORE_ERROR("Failed to parse Asset Registry file '{0}'\n      {1}", path.string(), e.what());
			return false;
		}

		auto rootNode = data["AssetRegistry"];
		if (!rootNode)
			return false;

		m_AssetRegistry.clear();
		for (const auto& node : rootNode)
		{
			// Read and cast handle
			AssetHandle handle = static_cast<AssetHandle>(node["Handle"].as<uint64_t>());
			auto& metadata = m_AssetRegistry[handle];

			// Read metadata fields
			metadata.FilePath = node["FilePath"].as<std::string>();
			metadata.Type = AssetTypeFromString(node["Type"].as<std::string>());
		}

		SD_CORE_INFO("RuntimeAssetManager loaded {0} assets from registry.", m_AssetRegistry.size());
		return true;
	}
}
