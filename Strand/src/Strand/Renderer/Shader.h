#pragma once

#include <string>
#include <glm/glm.hpp>


namespace Strand {

	struct ShaderUniform
	{
		std::string Name;
		uint32_t Size = 0;
		uint32_t Offset = 0;
		// We could also store GLenum Type here if needed
	};

	// Struct to hold information about a uniform block (UBO)
	struct ShaderUniformBlock
	{
		std::string Name;
		uint32_t Size = 0;
		uint32_t BindingPoint = 0;
		std::vector<ShaderUniform> Uniforms;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;
		virtual const uint32_t GetShaderID() const = 0; 

		virtual const ShaderUniform& FindUniform(const std::string& name) const = 0;
		virtual const ShaderUniformBlock& FindUniformBlock(const std::string& name) const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};


	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}