#pragma once
#include <glm/glm.hpp>

namespace Strand {

    // Bound at BindingPoint::FrameConstants (0)
    struct FrameConstants {
        glm::mat4 ViewProjection;
        glm::mat4 InverseViewProjection;
        glm::mat4 View;
        glm::mat4 InverseView;
        glm::mat4 Projection;
        glm::mat4 InverseProjection;

        // alignas(16) forces C++ to align this exactly how std140 expects a vec3
        alignas(16) glm::vec3 CameraPosition;
        float _Padding0; // Fills the remaining 4 bytes of the vec3's 16-byte alignment

        // alignas(8) matches std140 vec2 alignment
        alignas(8) glm::vec2 ViewportSize;

        float Time;
        float DeltaTime;

        uint32_t FrameIndex;
        uint32_t NumPointLights;
        uint32_t NumDirectionalLights;
        uint32_t NumSpotLights;
    };
}