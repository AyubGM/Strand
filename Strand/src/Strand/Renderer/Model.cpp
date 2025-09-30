#include "sdpch.h"
#include "Model.h"

namespace Strand {

    Model::Model(const std::string& path, const Ref<Shader>& shader)
        : m_Shader(shader)
    {
        LoadModel(path);
    }


    void Model::LoadModel(std::string path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            SD_CORE_ERROR("ASSIMP ERROR: {0}", import.GetErrorString());
            return;
        }

        m_Directory = path.substr(0, path.find_last_of('/'));

        ProcessMaterials(scene);
        ProcessNode(scene->mRootNode, scene);
    }


    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        // Process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(ProcessMesh(mesh, scene));
        }
        // Recurse for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    void Model::ProcessMaterials(const aiScene* scene)
    {
        m_Materials.reserve(scene->mNumMaterials);
        for (uint32_t i = 0; i < scene->mNumMaterials; i++)
        {
            aiMaterial* material = scene->mMaterials[i];
            Ref<Material> newMaterial = Material::Create(m_Shader);

            // This is a simplified example. A full PBR material would load
            // Albedo, Normal, Metallic, Roughness, AO maps.
            LoadMaterialTextures(material, aiTextureType_DIFFUSE, "u_ModelDiffuseTexture", newMaterial);
            LoadMaterialTextures(material, aiTextureType_SPECULAR, "u_ModelSpecularTexture", newMaterial);
            // You could also load colors and float values here
            // aiColor3D color;
            // material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            // newMaterial->Set("u_Albedo", glm::vec3(color.r, color.g, color.b));

            m_Materials.push_back(newMaterial);
        }
    }

    Ref<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<StaticMeshVertex>  vertices;
        std::vector<uint32_t> indices;

        vertices.reserve(mesh->mNumVertices);
        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            StaticMeshVertex vertex;
            vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
            vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                vertex.TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
            }
            else 
            {
                vertex.TexCoords = glm::vec2(0.0f);
            }

          /*  if (mesh->mTangents)
            {
                vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
            }
            if (mesh->mBitangents)
            {
                vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
            }*/

            vertices.push_back(vertex);
        }

        // process indices
        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return CreateRef<Mesh>(std::move(vertices), std::move(indices), mesh->mMaterialIndex);
    }

    void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, Ref<Material>& material)
    {
        // A simple cache could be added here to avoid loading the same texture multiple times

            for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                std::string texturePath = m_Directory + '/' + std::string(str.C_Str());
                SD_CORE_TRACE("TexturePath: {0}", texturePath);
                Ref<Texture2D> texture = Texture2D::Create(texturePath);
                material->Set(typeName, texture);
            }

    }
}