#pragma once
#include "Strand/Renderer/Texture.h"

namespace Strand {

	class OpenGLTextureLoader
	{
	public:
		static Ref<Texture2D> LoadTexture(const std::string& path);

	private:
		static std::unordered_map<std::string, Ref<Texture2D>> s_TextureCache;
	};

	
}