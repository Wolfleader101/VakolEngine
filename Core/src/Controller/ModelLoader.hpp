#pragma once

#include <vector>

#include <assimp/scene.h>

#include <Model/Buffer.hpp>
#include <Model/Material.hpp>

using Vakol::Model::Math::Vec3;
using Vakol::Model::Math::Vec2;

using Vakol::Model::Vertex;
using Vakol::Model::Texture;

using Vakol::Model::MaterialInfo;

namespace Vakol::Controller
{
    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const MaterialInfo& materialInfo)
        {
            this->vertices = vertices;
            this->indices = indices;
            this->materialInfo = materialInfo;
        }

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        MaterialInfo materialInfo;
    };

    class ModelLoader
    {
    public:
        static std::vector<Texture> textures_loaded;

        static std::vector<Mesh> meshes;

        static void LoadModel(const std::string& path);
    private:
        ModelLoader() {};

        static void ProcessNode(aiNode* node, const aiScene* scene);

        static Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

        static MaterialInfo ProcessMaterial(aiMaterial* material, const std::vector<Texture>& textures);

        static std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

    private:
        static std::string directory;
    };
}