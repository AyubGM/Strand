#include "sdpch.h"
#include "Material.h"

namespace Strand {

	uint32_t Material::s_MaterialCount = 0;



	Material::Material(const Ref<Shader>& shader)
		: m_Shader(shader)
	{
		// Assign the unique ID
		m_MatiralID = s_MaterialCount++;
	}

	Material::Material(const Ref<Shader>& shader, const std::string& albedoMap, const std::string& normalMap,
		const std::string& metallicMap, const std::string& roughnessMap,
		const std::string& aoMap)
		: m_Shader(shader), m_HasTextures(true)
	{
		// Assign the unique ID
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
	}

	Material::Material(const Ref<Shader>& shader, const glm::vec3& albedo, float metallic, float roughness, float ao)
		: m_Shader(shader), m_Albedo(albedo), m_Metallic(metallic), m_Roughness(roughness), m_AO(ao), m_HasTextures(false)
	{
		// Assign the unique ID
		m_MatiralID = s_MaterialCount++;

		// Store values in the generic maps
		m_Vec4s["u_Albedo"] = glm::vec4(albedo, 1.0f);
		m_Floats["u_Metallic"] = metallic;
		m_Floats["u_Roughness"] = roughness;
		m_Floats["u_Ao"] = ao;
	}

	void Material::Bind() const
	{
		m_Shader->Bind();

		// Bind all textures
		uint32_t textureSlot = 0;
		for (const auto& [name, texture] : m_Textures)
		{
			texture->Bind(textureSlot++);
			m_Shader->SetInt(name, texture->GetTextureSlot());
		}

		// Set all float uniforms
		for (const auto& [name, value] : m_Floats)
		{
			m_Shader->SetFloat(name, value);
		}

		// Set all vec4 uniforms
		for (const auto& [name, value] : m_Vec4s)
		{
			m_Shader->SetFloat4(name, value);
		}
	}

	void Material::UnBind() const
	{
		m_Shader->Unbind();
	}

	template<typename T>
	void Material::Set(const std::string& name, const T& value)
	{
		GetMapForType<T>(this)[name] = value;
	}

	template void Material::Set<float>(const std::string&, const float&);
	template void Material::Set<glm::vec4>(const std::string&, const glm::vec4&);
	template void Material::Set<Ref<Texture2D>>(const std::string&, const Ref<Texture2D>&);


	Ref<Material> Material::Create(const Ref<Shader>& shader)
	{
		return CreateRef<Material>(shader);
	}

}