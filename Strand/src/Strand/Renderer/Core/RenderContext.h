#pragma once

#include "FrameConstants.h"
#include "Strand/Core/Base.h"
#include "Strand/Renderer/RHI/FramebufferT.h"
#include "Strand/Renderer/RHI/UniformBufferT.h"
#include "Strand/Renderer/RHI/StorageBuffer.h"
#include "RenderQueue.h"


#include <glm/glm.hpp>


namespace Strand
{
	class RenderGraph;

	struct RenderContext
	{
		// Viewport Configuration
		FrameConstants FrameData;
		Ref<FramebufferT> TargetFramebuffer; // Does it have to be a sharedpointer? Should it be a uniquepointer instead?
		glm::vec4 ViewportRect = { 0.0f, 0.0f, 1280.0f, 720.0f };

		// GPU buffers for this viewport (allocated from Renderer pools)
		Ref<UniformBufferT> FrameConstantBuffer; // UBO at binding 0
		StorageBuffer ObjectSSBO;                 // SSBO at binding 3
		StorageBuffer DrawCommandSSBO;            // SSBO at binding 4
		StorageBuffer DrawCountBuffer;            // SSBO at binding 6 (indirect count)

		uint32_t MaxDrawCommands = 65536;
		uint32_t MaxRenderObjects = 65536;

		// CPU-side queue (filled by Scene, uploaded to ObjectSSBO)
		RenderQueue Queue;

		// Per-context render graph (owned by Renderer, assigned per viewport)
		RenderGraph* Graph = nullptr;

		void UploadFrameConstants();
		void UploadRenderObjects();
		void Reset();

	};
}