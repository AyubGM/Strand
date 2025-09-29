#pragma once

#include "Strand/Core/Base.h"
#include "Strand/Renderer/Shader.h"
#include "Strand/Renderer/Texture.h"
#include "Strand/Renderer/UniformBuffer.h"



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
        static Ref<Material> Create(const Ref<Shader>& shader);

        Material(const Ref<Shader>& shader, uint32_t materialID);


        ~Material() = default;

        void Bind() const;
        void UnBind() const;

        template<typename T>
        void Set(const std::string& name, const T& value);

        template<typename T>
        T Get(const std::string& name) const;

     /*   const Ref<Texture2D>& GetAlbedoMap() const { return m_AlbedoMap; }
        const Ref<Texture2D>& GetNormalMap() const { return m_NormalMap; }
        const Ref<Texture2D>& GetMetallicMap() const { return m_MetallicMap; }
        const Ref<Texture2D>& GetRoughnessMap() const { return m_RoughnessMap; }
        const Ref<Texture2D>& GetAOMap() const { return m_AOMap; }*/

        Ref<Texture2D> GetAlbedoMap() const { return Get<Ref<Texture2D>>("u_AlbedoMap"); }
        Ref<Texture2D> GetNormalMap() const { return Get<Ref<Texture2D>>("u_NormalMap"); }
        Ref<Texture2D> GetMetallicMap() const { return Get<Ref<Texture2D>>("u_MetallicMap"); }
        Ref<Texture2D> GetRoughnessMap() const { return Get<Ref<Texture2D>>("u_RoughnessMap"); }
        Ref<Texture2D> GetAOMap() const { return Get<Ref<Texture2D>>("u_AOMap"); }

        glm::vec3 GetAlbedo() const { return Get<glm::vec3>("u_Material.Albedo"); }
        float GetMetallic() const { return Get<float>("u_Material.Metallic"); }
        float GetRoughness() const { return Get<float>("u_Material.Roughness"); }
        float GetAO() const { return Get<float>("u_Material.AO"); }

        uint32_t GetID() const { return m_MaterialID; }
        Ref<Shader> GetShader() { return m_Shader; }
        bool HasTextures() const { return !m_Textures.empty(); }

        
    private:
        // Helper function to get the correct map for a given type
        template<typename T>
        std::unordered_map<std::string, T>& GetMapForType();

        template<typename T>
        const std::unordered_map<std::string, T>& GetConstMapForType() const;


    private:
        uint32_t  m_MaterialID;
        Ref<Shader> m_Shader;

        // --- UBO-related members ---
        Ref<UniformBuffer> m_UniformBuffer;
        std::vector<uint8_t> m_CPUBuffer; // CPU-side buffer to stage data
        mutable bool m_IsDirty = true;    // Track if UBO needs updating


        std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
       // mutable uint32_t m_NextTextureSlot = 0;

        std::unordered_map<std::string, glm::vec4> m_Vec4s;
        std::unordered_map<std::string, glm::vec3> m_Vec3s;
        std::unordered_map<std::string, float> m_Floats;
        std::unordered_map<std::string, int> m_Ints;

        static uint32_t s_MaterialCount;
    };

    template<typename T>
    void Material::Set(const std::string& name, const T& value)
    {
       // auto& map = GetMapForType<T>();
       //map[name] = value;
        // Handle Textures(samplers) separately as they are not part of the UBO
        if constexpr (std::is_same_v<T, Ref<Texture2D>>)
        {
            m_Textures[name] = value;
        }
        else // Handle all UBO data
        {
            // Get the uniform's offset and size from the shader's reflection data
            const auto& uniform = m_Shader->FindUniform(name);
            if (uniform.Size == 0) // Uniform not found or not in a UBO
            {
                // Log a warning: uniform 'name' not found in material shader
                SD_CORE_ERROR("Uinform: {0} not found in the {1} Shader", name, m_Shader->GetName());
                return;
            }

            // Write the data into our CPU-side buffer at the correct offset
            memcpy(m_CPUBuffer.data() + uniform.Offset, &value, uniform.Size);
            m_IsDirty = true;
        }
    }

    template<typename T>
    T Material::Get(const std::string& name) const
    {
        const auto& map = GetConstMapForType<T>();
        auto it = map.find(name);
        if (it != map.end())
        {
            return it->second;
        }
        // Return a default-constructed value if not found.
        // For Ref<Texture2D>, this will be nullptr. For float, it will be 0.0f.
        return T{};
    }
}
