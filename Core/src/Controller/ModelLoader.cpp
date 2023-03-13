#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <Model/GL/GLTexture.hpp>

#include "Logger.hpp"
#include "ModelLoader.hpp"

using Vakol::Model::GetTexture;

namespace Vakol::Controller
{
    std::vector<Texture> ModelLoader::textures_loaded;
    std::vector<Mesh> ModelLoader::meshes;

    std::string ModelLoader::directory;

    void ModelLoader::LoadModel(const std::string& path)
    {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            VK_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);
    }

    void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Mesh ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;

            vertex.position = vector;

            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;

                vertex.normal = vector;
            }

            if (mesh->mTextureCoords[0])
            {
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
            }
            else
                vertex.uv = glm::vec2(0.0f);

            vertices.push_back(vertex);
        }

        // faces
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }

        // materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // Diffuse Maps
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        // Specular Maps
        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        // Normal Maps
        std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        // Emissive Maps
        std::vector<Texture> emissiveMaps = LoadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
        textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());

        VK_TRACE("{0}", textures.size());

        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> ModelLoader::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
    {
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // prevents already loaded textures from being loaded again
            bool skip = false;

            for (unsigned int j = 0; j < textures_loaded.size(); ++j)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    
                    skip = true;

                    break;
                }
            }

            if (!skip) // if texture has not already been loaded
            {
                Texture texture;
                texture.id = GetTexture((directory + "/textures/" + str.C_Str()).c_str(), true); // this might need to change
                texture.type = typeName;
                texture.path = str.C_Str();

                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }

        return textures;
    }
}