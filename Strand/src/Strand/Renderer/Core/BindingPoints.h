#pragma
#include <cstdint>

namespace Strand {

	namespace BindingPoint
	{
        //SET 0: GLOBAL FRAME DATA (Bound once per frame)
        constexpr uint32_t FrameConstants = 0;  // View, Proj, Time, Camera
        constexpr uint32_t LightBuffer = 1;  // Array of all lights in the scene
        constexpr uint32_t ShadowMaps = 2;  // Array of shadow map depth textures
        constexpr uint32_t GlobalTextures = 3;  // Bindless array of all loaded textures

        // SET 1: SCENE DATA (Bound once per scene/pass)
        constexpr uint32_t RenderObjectBuffer = 4;  // SSBO: Transforms, AABBs, Material IDs
        constexpr uint32_t MaterialSSBO = 5;  // SSBO: Colors, Roughness, Texture Indices

        // SET 2: COMPUTE CULLING & INDIRECT (Bound for compute + draw)
        constexpr uint32_t DrawCommandBuffer = 6;  // SSBO: Output of culling (the draw commands)
        constexpr uint32_t DrawCountBuffer = 7;  // SSBO: Atomic counter of visible objects
        constexpr uint32_t VisibleIndexBuffer = 8;  // SSBO: 2-Pass occlusion visibility data
        constexpr uint32_t HZBTexture = 9;  // Image: Hierarchical Z-Buffer for occlusion

        // SET 3: LEGACY / FALLBACKS (Phase these out eventually)
        constexpr uint32_t LegacyObjectData = 10;
        constexpr uint32_t LegacyMaterialData = 11;
	};
}
