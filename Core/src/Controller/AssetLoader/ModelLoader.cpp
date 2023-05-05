#include "ModelLoader.hpp"

#include <vector>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <Controller/Logger.hpp>

#include <chrono>

using namespace Vakol::Model::Assets;

using Vakol::Model::Vertex;

glm::vec3 to_glm(const aiColor3D& val) { return { val.r, val.g, val.b }; }

void ProcessNode(const aiNode* node, const aiScene* scene);
Vakol::Model::Assets::Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);
MaterialSpec ProcessMaterial(const aiMaterial* mat);

std::vector<Texture> LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, const std::string& typeName);

std::string directory;

std::vector<Vakol::Model::Assets::Mesh> meshes;
std::vector<Texture> textures_loaded;

namespace Vakol::Controller
{
    ::Model LoadModel(const std::string& path) 
    {
        Assimp::Importer importer;

        VK_TRACE("Loading Model: {0}", path);

        const auto start = std::chrono::steady_clock::now();

        const aiScene* scene = importer.ReadFile(
            path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            VK_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
            quick_exit(EXIT_FAILURE);
        }

        directory = path.substr(0, path.find_last_of('/'));

        ::ProcessNode(scene->mRootNode, scene);

        const auto end = std::chrono::steady_clock::now();

        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        VK_TRACE("Finished Loading Model. Elapsed Time: {0} ms", duration.count());

        return { meshes };
    }
}

void ProcessNode(const aiNode* node, const aiScene* scene) 
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) 
    {
        const auto mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
        ProcessNode(node->mChildren[i], scene);
}

Vakol::Model::Assets::Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene) 
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) 
    {
        Vertex vertex{};
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;

        vertex.position = vector;

        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;

            vertex.normal = vector;
        }

        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;

            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;

            vertex.uv = vec;

            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;

            vertex.tangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;

            vertex.bitangent = vector;
        } else
            vertex.uv = glm::vec2(0.0f);

        vertices.push_back(vertex);
    }

    // faces
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) 
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; ++j) indices.push_back(face.mIndices[j]);
    }

    // material
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    return { vertices, indices, sizeof(Vertex), ProcessMaterial(material) };
}

MaterialSpec ProcessMaterial(const aiMaterial* mat) 
{
    std::vector<Texture> textures;

    aiColor3D ambient, diffuse, specular, emissive;
    float shininess;

    mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    mat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    mat->Get(AI_MATKEY_SHININESS, shininess);

    // Diffuse Maps
    std::vector<Texture> diffuse_maps = LoadMaterialTextures(mat, aiTextureType_DIFFUSE, "diffuse_map");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

    // Specular Maps
    std::vector<Texture> specular_maps = LoadMaterialTextures(mat, aiTextureType_SPECULAR, "specular_map");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

    // Normal Maps
    std::vector<Texture> normal_maps = LoadMaterialTextures(mat, aiTextureType_NORMALS, "normal_map");
    textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

    // Emissive Maps
    std::vector<Texture> emissive_maps = LoadMaterialTextures(mat, aiTextureType_EMISSIVE, "emissive_map");
    textures.insert(textures.end(), emissive_maps.begin(), emissive_maps.end());

    return { to_glm(ambient), to_glm(diffuse), to_glm(specular), to_glm(emissive), shininess, textures };
}

std::vector<Texture> LoadMaterialTextures(const aiMaterial* mat, const aiTextureType type, const std::string& typeName) 
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) 
    {
        aiString str;

        mat->GetTexture(type, i, &str);

        // prevents already loaded textures from being loaded again
        bool skip = false;

        for (const auto& texture : textures_loaded)
        {
            if (std::strcmp(texture.path.data(), str.C_Str()) == 0) 
            {
                textures.push_back(texture);

                skip = true;

                break;
            }
        }

        if (!skip)  // if texture has not already been loaded
        {
            Texture&& texture {};

            // Don't load ID yet.
            texture.path = str.C_Str();
            texture.type = typeName;

            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}
