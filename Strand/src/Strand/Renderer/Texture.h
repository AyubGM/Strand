#pragma once

#include <string>

#include "Strand/Core/Base.h"
#include "Strand/Core/Buffer.h"

#include "Strand/Asset/Asset.h"

namespace Strand {

	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;
	};

	class Texture : public Asset
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(Buffer data) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const TextureSpecification& specification, Buffer data = Buffer());

		static  AssetType GetStaticType() { return AssetType::Texture2D; }
		virtual AssetType GetType() const { return GetStaticType(); }

	};

	class Texture3D : public Texture
	{
	public:
		static Ref<Texture3D> Create(uint32_t width, uint32_t height, uint32_t depth);
		static Ref<Texture3D> Create(const std::vector<std::string>& paths);

		static  AssetType GetStaticType() { return AssetType::Texture3D; }
		virtual AssetType GetType() const { return GetStaticType(); }

		virtual uint32_t GetDepth() const = 0;
		virtual const std::vector<std::string>& GetPaths() const = 0;


	};

	class TextureCube : public Texture
	{
	public:

		static Ref<TextureCube> Create(const std::vector<std::string>& paths);
		static  AssetType GetStaticType() { return AssetType::TextureCube; }
		virtual AssetType GetType() const { return GetStaticType(); }

		virtual const std::vector<std::string>& GetPaths() const = 0;
	};

}