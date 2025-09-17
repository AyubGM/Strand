#include "sdpch.h"
#include "OpenGLTextureLoader.h"
#include "OpenGLTexture.h"

namespace Strand {

	std::unordered_map<std::string, Ref<Texture2D>> OpenGLTextureLoader::s_TextureCache;

    Ref<Texture2D> OpenGLTextureLoader::LoadTexture(const std::string& path)
    {
        if (s_TextureCache.find(path) != s_TextureCache.end())
        {
            return s_TextureCache[path];
        }

        Ref<Texture2D> texture = CreateRef<OpenGLTexture2D>(path);

        if (texture->IsLoaded())
        {
            s_TextureCache[path] = texture;
            return texture;
        }

        return nullptr;
    }

}

