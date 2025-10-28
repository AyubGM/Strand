#pragma once

#include "Strand/Renderer/Texture.h"
#include <glad/glad.h>

namespace Strand {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpecification& specification, Buffer data = Buffer());
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(Buffer data) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}
	private:
		TextureSpecification m_Specification;

		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};


	class OpenGLTexture3D : public Texture3D
	{
	public:
		OpenGLTexture3D(uint32_t width, uint32_t height, uint32_t depth);
		OpenGLTexture3D(const std::vector<std::string>& paths);
		virtual ~OpenGLTexture3D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetDepth() const override { return m_Depth; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		//virtual void SetData(void* data, uint32_t size) override;
		virtual void SetData(Buffer data) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual const std::vector<std::string>& GetPaths() const override { return m_Paths; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}
	private:
		std::vector<std::string> m_Paths;
		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height, m_Depth;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(const std::vector<std::string>& faces);
		virtual ~OpenGLTextureCube();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		//virtual void SetData(void* data, uint32_t size) override;
		virtual void SetData(Buffer data) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual const std::vector<std::string>& GetPaths() const override { return m_Paths; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.GetRendererID();
		}

	private:
		std::vector<std::string> m_Paths;
		bool m_IsLoaded = false;
		uint32_t m_Width = 0, m_Height = 0;
		uint32_t m_RendererID = 0;
		GLenum m_InternalFormat = 0, m_DataFormat = 0;
	};

}