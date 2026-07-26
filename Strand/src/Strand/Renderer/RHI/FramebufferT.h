#pragma once

#include "Strand/Core/Base.h"

namespace Strand {

	enum class FramebufferTextureFormatT
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecificationT
	{
		FramebufferTextureSpecificationT() = default;
		FramebufferTextureSpecificationT(FramebufferTextureFormatT format)
			: TextureFormat(format) {
		}

		FramebufferTextureFormatT TextureFormat = FramebufferTextureFormatT::None;
		// TODO: filtering/wrap
	};

	struct FramebufferAttachmentSpecificationT
	{
		FramebufferAttachmentSpecificationT() = default;
		FramebufferAttachmentSpecificationT(std::initializer_list<FramebufferTextureSpecificationT> attachments)
			: Attachments(attachments) {
		}

		std::vector<FramebufferTextureSpecificationT> Attachments;
	};

	struct FramebufferSpecificationT
	{
		uint32_t Width, Height;
		FramebufferAttachmentSpecificationT Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class FramebufferT
	{
	public:
		virtual ~FramebufferT() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecificationT& GetSpecification() const = 0;
	};


}