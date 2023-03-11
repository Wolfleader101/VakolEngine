#pragma once

#include <vector>

#include <assimp/scene.h>

#include <Model/Buffer.hpp>

using Vakol::Model::Math::Vec3;
using Vakol::Model::Math::Vec2;

using Vakol::Model::Vertex;

namespace Vakol::Controller
{
    struct Texture
    {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
    };

    class ModelLoader
    {
    public:
        static std::vector<Texture> textures_loaded;
        static std::vector<Mesh> meshes;

        static std::string directory;

        static void LoadModel(const std::string& path);
    private:
        ModelLoader() {};

        static void ProcessNode(aiNode* node, const aiScene* scene);

        static Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

        static std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
    };
}