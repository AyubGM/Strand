#include "sdpch.h"
#include "MaterialLibrary.h"

namespace Strand {

    void MaterialLibrary::Add(const std::string& name, const Ref<Material>& material)
    {
        if (Exists(name))
        {
            SD_CORE_WARN("Material '{0}' already exists in the library. Cannot add a duplicate.", name);
            return;
        }
        m_Materials[name] = material;
    }

    void MaterialLibrary::Add(const Ref<Material>& material)
    {
        // Example: Naming the material based on its shader's name + its ID
        const auto& name = material->GetShader()->GetName() + std::to_string(material->GetID());
        Add(name, material);
    }

    Ref<Material> MaterialLibrary::Get(const std::string& name)
    {
        if (!Exists(name))
        {
            //TODO
            SD_CORE_ERROR("Material '{0}' not found in the library.", name);
            return nullptr;
        }
        return m_Materials.at(name);
    }

    bool MaterialLibrary::Exists(const std::string& name) const
    {
        return m_Materials.find(name) != m_Materials.end();
    }

}
