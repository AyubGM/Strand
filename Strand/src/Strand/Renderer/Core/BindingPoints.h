#pragma
#include <cstdint>

namespace Strand {

	enum class BindingPoints : uint32_t
	{
		// UBOs
		FrameConstants = 0,

		// SSBOs
		RenderObjectBuffer = 1,
		DrawCommandBuffer = 2,
		MeteraialSSBO = 3,
		VisibleIndexBuffer = 4,

		// Textures (compute write)
		HZBTexture = 5,
		ShadowMapTexture = 6,

		Count
	};
}
