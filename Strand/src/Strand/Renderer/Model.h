#pragma once

#include "Strand/Renderer/Mesh.h"
#include "Strand/Renderer/Material.h"
#include "Strand/Renderer/Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

namespace Strand {

    class Model
    {

    public:
        
        Model(const std::string& path, const Ref<Shader>& shader);

        const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }
        const std::vector<Ref<Material>>& GetMaterials() const { return m_Materials; }

    private:
        void LoadModel(std::string path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        void ProcessMaterials(const aiScene* scene);

        void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, Ref<Material>& material);
       
    private:
        // model data
        std::vector<Ref<Mesh>> m_Meshes;
        std::vector<Ref<Material>> m_Materials;
        std::string m_Directory;
        Ref<Shader> m_Shader;
        std::vector<StaticMeshTexture> m_Textures_loaded;

   
    };
}