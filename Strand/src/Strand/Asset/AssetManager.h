#pragma once

#include "AssetManagerBase.h"

#include "Strand/Project/Project.h"

namespace Strand {

	class AssetManager
	{
	public:
		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			//Ref<Asset> asset = Project::GetActive()->GetAssetManager()->GetAsset(handle);

			Ref<Asset> asset = GetProjectAssetManager()->GetAsset(handle);
			return std::static_pointer_cast<T>(asset);
		}

		static bool IsAssetHandleValid(AssetHandle handle)
		{
			//return Project::GetActive()->GetAssetManager()->IsAssetHandleValid(handle);
			return GetProjectAssetManager()->IsAssetHandleValid(handle);
		}

		static bool IsAssetLoaded(AssetHandle handle)
		{
			//return Project::GetActive()->GetAssetManager()->IsAssetLoaded(handle);
			return GetProjectAssetManager()->IsAssetLoaded(handle);
		}

		static AssetType GetAssetType(AssetHandle handle)
		{
			//return Project::GetActive()->GetAssetManager()->GetAssetType(handle);
			return GetProjectAssetManager()->GetAssetType(handle);
		}

	private:
		static std::shared_ptr<AssetManagerBase> GetProjectAssetManager()
		{
			SD_CORE_ASSERT(Project::GetActive(), "AssetManager access failed: No active project found!");
			return Project::GetActive()->GetAssetManager();
		}
	};

}