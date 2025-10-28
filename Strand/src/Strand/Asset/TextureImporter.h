#pragma once

#include "Asset.h"
#include "AssetMetadata.h"

#include "Strand/Renderer/Texture.h"

namespace Strand {

	class TextureImporter
	{
	public:
		static Ref<Texture2D> ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata);
	};



}