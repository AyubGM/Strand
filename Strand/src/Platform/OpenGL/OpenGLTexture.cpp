#include "sdpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Strand {

	namespace Utils {

		static GLenum GetCubemapTarget(uint32_t faceIndex)
		{
			return GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex;
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		SD_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		SD_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			SD_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std:string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		/*SD_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}*/

		if (data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			SD_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		SD_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		SD_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		SD_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		SD_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}

	//--------TEXTURE3D---------------------//

	OpenGLTexture3D::OpenGLTexture3D(uint32_t width, uint32_t height, uint32_t depth)
		: m_Width(width), m_Height(height), m_Depth(depth)
	{
		SD_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_3D, 1, &m_RendererID);
		glTextureStorage3D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height, m_Depth);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_IsLoaded = true;
	}


	OpenGLTexture3D::OpenGLTexture3D(const std::vector<std::string>& paths)
		: m_Paths(paths)
	{
		SD_PROFILE_FUNCTION();

		if (paths.empty())
		{
			SD_CORE_ERROR("Cannot create Texture3D from an empty list of paths.");
			return;
		}

		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;
		stbi_uc* first_slice_data = nullptr;
		{
			SD_PROFILE_SCOPE("stbi_load - OpenGLTexture3D::OpenGLTexture3D(const std:string&)");
			first_slice_data = stbi_load(paths[0].c_str(), &width, &height, &channels, 0);
		}

		SD_CORE_ASSERT(first_slice_data, "Failed to load image for 3D texture's first slice");

		m_Width = width;
		m_Height = height;
		m_Depth = static_cast<uint32_t>(paths.size());

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		SD_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		size_t slice_size = m_Width * m_Height * channels;
		size_t total_size = slice_size * m_Depth;
		stbi_uc* all_slices_data = (stbi_uc*)malloc(total_size);
		SD_CORE_ASSERT(all_slices_data, "Failed to allocate memory for 3D texture data!");

		memcpy(all_slices_data, first_slice_data, slice_size);
		stbi_image_free(first_slice_data);

		for (uint32_t i = 1; i < m_Depth; ++i)
		{
			int current_w, current_h, current_c;
			stbi_uc* slice_data = stbi_load(paths[i].c_str(), &current_w, &current_h, &current_c, 0);
			if (!slice_data)
			{
				SD_CORE_ERROR("Failed to load image for 3D texture slice %d: %s", i, paths[i].c_str());
				free(all_slices_data);
				return;
			}

			if (current_w != m_Width || current_h != m_Height || current_c != channels)
			{
				SD_CORE_ERROR("3D Texture slice %d has mismatched dimensions/format!", i);
				stbi_image_free(slice_data);
				free(all_slices_data);
				return;
			}

			memcpy(all_slices_data + (i * slice_size), slice_data, slice_size);
			stbi_image_free(slice_data);
		}

		glCreateTextures(GL_TEXTURE_3D, 1, &m_RendererID);
		glTextureStorage3D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height, m_Depth);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTextureSubImage3D(m_RendererID, 0, 0, 0, 0, m_Width, m_Height, m_Depth, m_DataFormat, GL_UNSIGNED_BYTE, all_slices_data);

		free(all_slices_data);
		m_IsLoaded = true;

	}
		
	OpenGLTexture3D::~OpenGLTexture3D()
	{
		SD_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture3D::SetData(void* data, uint32_t size)
	{
		SD_PROFILE_FUNCTION();
		uint32_t bpp = (m_DataFormat == GL_RGBA) ? 4 : 3;
		SD_CORE_ASSERT(size == m_Width * m_Height * m_Depth * bpp, "Data must be for the entire 3D texture!");
		glTextureSubImage3D(m_RendererID, 0, 0, 0, 0, m_Width, m_Height, m_Depth, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture3D::Bind(uint32_t slot) const
	{
		SD_PROFILE_FUNCTION();
		glBindTextureUnit(slot, m_RendererID);
	}


	//--------TEXTURECUBE---------------------//

	OpenGLTextureCube::OpenGLTextureCube(const std::vector<std::string>& faces)
		: m_Paths(faces)
	{
		SD_PROFILE_FUNCTION();

		SD_CORE_ASSERT(faces.size() == 6, "A cubemap requires exactly 6 texture paths.");

		stbi_set_flip_vertically_on_load(0);

		int width, height, channels;
		stbi_uc* firstFaceData = stbi_load(faces[0].c_str(), &width, &height, &channels, 0);
		if (!firstFaceData)
		{
			SD_CORE_ERROR("Failed to load cubemap face: {0}", faces[0]);
			stbi_image_free(firstFaceData);
			return;
		}

		m_Width = width;
		m_Height = height;

		if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else
		{
			SD_CORE_ERROR("Unsupported image format (channels={0}) for cubemap face: {1}", channels, faces[0]);
			stbi_image_free(firstFaceData);
			return;
		}


		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTextureSubImage3D(m_RendererID, 0, 0, 0, Utils::GetCubemapTarget(0), m_Width, m_Height, 1, m_DataFormat, GL_UNSIGNED_BYTE, firstFaceData);
		stbi_image_free(firstFaceData);

		for (uint32_t i = 1; i < faces.size(); i++)
		{
			int faceWidth, faceHeight, faceChannels;
			stbi_uc* faceData = stbi_load(faces[i].c_str(), &faceWidth, &faceHeight, &faceChannels, 0);
			if (!faceData)
			{
				SD_CORE_ERROR("Failed to load cubemap face: {0}", faces[i]);
				free(faceData);
				return;
			}

			if (faceWidth != m_Width || faceHeight != m_Height || faceChannels != channels)
			{
				SD_CORE_WARN("Cubemap face '{0}' has mismatched dimensions/format. Skipping.", faces[i]);
				stbi_image_free(faceData);
				return;
			}

			glTextureSubImage3D(m_RendererID, 0, 0, 0, Utils::GetCubemapTarget(i), m_Width, m_Height, 1, m_DataFormat, GL_UNSIGNED_BYTE, faceData);
			stbi_image_free(faceData);
		}

		m_IsLoaded = true;
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		SD_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureCube::SetData(void* data, uint32_t size)
	{
		SD_PROFILE_FUNCTION();
		// This function is less common for cubemaps loaded from files, as it would need
		// data for all 6 faces. We assert to indicate it's not a typical use case.
		uint32_t bpp = (m_DataFormat == GL_RGBA) ? 4 : 3;
		SD_CORE_ASSERT(size == m_Width * m_Height * 6 * bpp, "SetData for cubemap requires data for all 6 faces!");
		// If you needed to implement this, you would call glTextureSubImage3D here.
	}

	void OpenGLTextureCube::Bind(uint32_t slot) const
	{
		SD_PROFILE_FUNCTION();
		glBindTextureUnit(slot, m_RendererID);
	}


}

