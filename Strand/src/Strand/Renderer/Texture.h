#pragma once

#include <string>

#include "Strand/Core/Base.h"

namespace Strand {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);

		virtual const std::string& GetPath() const = 0;

	};

	class Texture3D : public Texture
	{
	public:
		static Ref<Texture3D> Create(uint32_t width, uint32_t height, uint32_t depth);
		static Ref<Texture3D> Create(const std::vector<std::string>& paths);

		virtual uint32_t GetDepth() const = 0;
		virtual const std::vector<std::string>& GetPaths() const = 0;


	};

	class TextureCube : public Texture
	{
	public:

		static Ref<TextureCube> Create(const std::vector<std::string>& paths);
		virtual const std::vector<std::string>& GetPaths() const = 0;
	};

	class TextureLoader
	{
	public:
		static Ref<Texture2D> LoadTexture(const std::string& path);

	};
}