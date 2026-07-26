#pragma once
#include <glm/glm.hpp>
#include <cstdint>

namespace Strand {
	// Bound at BindingPoints::RenderObjectBuffer (1)
	// Layout: std430, 192 bytes, 16-byte aligned
	struct alignas(16) RenderObject {
		glm::mat4 Transform;
		glm::mat4 NormalMatrix;
		glm::vec4 AABBMin;        // w unused
		glm::vec4 AABBMax;        // w unused
		uint32_t MeshID;
		uint32_t MaterialID;
		uint32_t InstanceCount;
		uint32_t _padding[3];     // Pad to 192 bytes (16 * 12)
	};

	static_assert(sizeof(RenderObject) % 16 == 0, "RenderObject must be aligned to 16 bytes");
	static_assert(sizeof(RenderObject) == 192, "RenderObject must be 192 bytes in size");

	// Matches GL_DRAW_INDIRECT_BUFFER / DrawElementsIndirectCommand
	struct DrawCommand
	{
		uint32_t Count;  // The number of indices (vertices)
		uint32_t InstanceCount;
		uint32_t FirstIndex;
		int32_t BaseVertex;
		uint32_t BaseInstance;
	};

	// CPU-side sort key for binning (material front, depth back)
	struct RenderSortKey
	{
		uint64_t Value = 0;

		static RenderSortKey Build(uint32_t materialID, uint32_t depth)
		{
			RenderSortKey key;

			uint32_t intDepth = std::bit_cast<uint32_t>(depth);
			//[ 32 bits: Material ID ] [ 32 bits: Depth ]
			key.Value = (static_cast<uint64_t>(materialID) << 32) | intDepth;
			return key;
		}
	};
}