#pragma once

#include "Strand/Core/Base.h"
#include "Material.h"
#include <unordered_map>

namespace Strand {

    class MaterialLibrary
    {
    public:
        void Add(const std::string& name, const Ref<Material>& material);
        void Add(const Ref<Material>& material); // Adds with a default name or path

        Ref<Material> Get(const std::string& name);
        bool Exists(const std::string& name) const;

    private:
        std::unordered_map<std::string, Ref<Material>> m_Materials;
    };

}
