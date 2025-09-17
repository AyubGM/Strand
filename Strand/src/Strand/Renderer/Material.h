#pragma once

#include "Strand/Core/Base.h"
#include "Strand/Renderer/Shader.h"
#include "Strand/Renderer/Texture.h"

#include <glm/glm.hpp>
#include <unordered_map>

namespace Strand {

    enum class ShaderType
    {
        vertex = 0,
        hull,
        domain,
        geometry,
        pixel,
        compute,
        amplification,
        mesh,

        count
    };

    enum class MaterailType
    {
        opaque,
        // transparent, unlit, clear_coat, cloth, skin, foliage, hair

        count
    };

    struct Material_init_info
    {
        MaterailType type;
        uint32_t     textureCount; //NOTE: texture are optional, so, texture count may be 0 and texture_ids may be null
        uint32_t     shader_ids[static_cast<size_t>(ShaderType::count)]{0,0,0,0,0,0,0,0};
        uint32_t*     texture_ids;
    };

    class Material
    {
    public:
        Material(const Ref<Shader>& shader);
        Material(const Ref<Shader>& shader, const std::string& albedoMap, const std::string& normalMap,
            const std::string& metallicMap, const std::string& roughnessMap,
            const std::string& aoMap);
        Material(const Ref<Shader>& shader, const glm::vec3& albedo, float metallic, float roughness, float ao);

        ~Material() = default;

        void Bind() const;
        void UnBind() const;

        template<typename T>
        void Set(const std::string& name, const T& value);

        const Ref<Shader> GetShader() { return m_Shader; }
        const Ref<Texture2D>& GetAlbedoMap() const { return m_AlbedoMap; }
        const Ref<Texture2D>& GetNormalMap() const { return m_NormalMap; }
        const Ref<Texture2D>& GetMetallicMap() const { return m_MetallicMap; }
        const Ref<Texture2D>& GetRoughnessMap() const { return m_RoughnessMap; }
        const Ref<Texture2D>& GetAOMap() const { return m_AOMap; }

        const glm::vec3& GetAlbedo() const { return m_Albedo; }
        float GetMetallic() const { return m_Metallic; }
        float GetRoughness() const { return m_Roughness; }
        float GetAO() const { return m_AO; }

        bool HasTextures() const { return m_HasTextures; }


        static Ref<Material> Create(const Ref<Shader>& shader);

    private:
        // Helper function to get the correct map for a given type
        template<typename T>
        std::unordered_map<std::string, T>& GetMapForType(Material* material);

        template<>
        std::unordered_map<std::string, Ref<Texture2D>>& GetMapForType(Material* material) { return material->m_Textures; }

        template<>
        std::unordered_map<std::string, glm::vec4>& GetMapForType(Material* material) { return material->m_Vec4s; }

        template<>
        std::unordered_map<std::string, float>& GetMapForType(Material* material) { return material->m_Floats; }


    private:
        uint32_t  m_MatiralID;
        Ref<Shader> m_Shader;

        Ref<Texture2D> m_AlbedoMap;
        Ref<Texture2D> m_NormalMap;
        Ref<Texture2D> m_MetallicMap;
        Ref<Texture2D> m_RoughnessMap;
        Ref<Texture2D> m_AOMap;

        glm::vec3 m_Albedo;
        float m_Metallic;
        float m_Roughness;
        float m_AO;

        bool m_HasTextures = false;

        std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
        std::unordered_map<std::string, glm::vec4> m_Vec4s;
        std::unordered_map<std::string, float> m_Floats;

        static uint32_t s_MaterialCount;
    };
}
