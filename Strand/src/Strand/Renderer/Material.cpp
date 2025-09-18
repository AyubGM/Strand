#include "sdpch.h"
#include "Material.h"

namespace Strand {

	uint32_t Material::s_MaterialCount = 0;


	Ref<Material> Material::Create(const Ref<Shader>& shader)
	{
		return CreateRef<Material>(shader, s_MaterialCount++);
	}

	Material::Material(const Ref<Shader>& shader, uint32_t materialID)
		: m_Shader(shader), m_MaterialID(materialID)
	{
	}

	/* {
		//// Assign the unique ID
		m_MatiralID = s_MaterialCount++;

		m_AlbedoMap = TextureLoader::LoadTexture(albedoMap);
		m_NormalMap = TextureLoader::LoadTexture(normalMap);
		m_MetallicMap = TextureLoader::LoadTexture(metallicMap);
		m_RoughnessMap = TextureLoader::LoadTexture(roughnessMap);
		m_AOMap = TextureLoader::LoadTexture(aoMap);

		// Store textures in the generic map as well for the template Set/Get functions
		m_Textures["u_AlbedoMap"] = m_AlbedoMap;
		m_Textures["u_NormalMap"] = m_NormalMap;
		m_Textures["u_MetallicMap"] = m_MetallicMap;
		m_Textures["u_RoughnessMap"] = m_RoughnessMap;
		m_Textures["u_AOMap"] = m_AOMap;
	}*/


	void Material::Bind() const
	{
		m_Shader->Bind();

		// Set all vec3 uniforms
		for (const auto& [name, value] : m_Vec3s)
		{
			m_Shader->SetFloat3(name, value);
		}

		// Set all vec4 uniforms
		for (const auto& [name, value] : m_Vec4s)
		{
			m_Shader->SetFloat4(name, value);
		}

		// Set all float uniforms
		for (const auto& [name, value] : m_Floats)
		{
			m_Shader->SetFloat(name, value);
		}

		// Set all int uniforms
		for (const auto& [name, value] : m_Ints)
		{
			m_Shader->SetInt(name, value);
		}

		// Bind all textures and set their sampler uniforms
		uint32_t textureSlot = 0;
		for (const auto& [name, texture] : m_Textures)
		{
			if (texture) // Ensure texture is not null
			{
				texture->Bind(textureSlot);
				m_Shader->SetInt(name, textureSlot);
				textureSlot++;
			}
		}
	}

	void Material::UnBind() const
	{
		m_Shader->Unbind();
	}


	// --- Template Specializations for GetMapForType ---
	// These specializations tell the Set<T> and Get<T> templates which map to use for which type.

	template<>
	std::unordered_map<std::string, Ref<Texture2D>>& Material::GetMapForType<Ref<Texture2D>>() { return m_Textures; }

	template<>
	std::unordered_map<std::string, glm::vec4>& Material::GetMapForType<glm::vec4>() { return m_Vec4s; }

	template<>
	std::unordered_map<std::string, glm::vec3>& Material::GetMapForType<glm::vec3>() { return m_Vec3s; }

	template<>
	std::unordered_map<std::string, float>& Material::GetMapForType<float>() { return m_Floats; }

	template<>
	std::unordered_map<std::string, int>& Material::GetMapForType<int>() { return m_Ints; }

	// --- Template Specializations for GetConstMapForType (for const correctness) ---

	template<>
	const std::unordered_map<std::string, Ref<Texture2D>>& Material::GetConstMapForType<Ref<Texture2D>>() const { return m_Textures; }

	template<>
	const std::unordered_map<std::string, glm::vec4>& Material::GetConstMapForType<glm::vec4>() const { return m_Vec4s; }

	template<>
	const std::unordered_map<std::string, glm::vec3>& Material::GetConstMapForType<glm::vec3>() const { return m_Vec3s; }

	template<>
	const std::unordered_map<std::string, float>& Material::GetConstMapForType<float>() const { return m_Floats; }

	template<>
	const std::unordered_map<std::string, int>& Material::GetConstMapForType<int>() const { return m_Ints; }
	

}