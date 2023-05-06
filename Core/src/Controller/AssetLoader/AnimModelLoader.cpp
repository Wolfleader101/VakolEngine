#include "AnimModelLoader.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>
#include <Controller/AssetLoader/TextureLoader.hpp>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/quaternion.hpp>
#pragma warning(pop)

#include <Controller/Logger.hpp>

using Vakol::Model::Vertex;

using Vakol::Model::Assets::Model;
using Vakol::Model::MAX_BONE_INFLUENCE;

namespace Vakol::Controller
{
    constexpr unsigned int ASSIMP_LOADER_OPTIONS =
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights;

    static glm::mat4 to_glm(const aiMatrix4x4& m)
    {
        const glm::vec4& c1{ m.a1, m.b1, m.c1, m.d1 };
        const glm::vec4& c2{ m.a2, m.b2, m.c2, m.d2 };
        const glm::vec4& c3{ m.a3, m.b3, m.c3, m.d3 };
        const glm::vec4& c4{ m.a4, m.b4, m.c4, m.d4 };

        return { std::move(c1), std::move(c2), std::move(c3), std::move(c4) };
    }

    static glm::quat to_glm(const aiQuaternion& q) { return { q.w, q.x, q.y, q.z }; }

    static glm::vec3 to_glm(const aiVector3D& v) { return { v.x, v.y, v.z }; }

    auto extract_vertices(const aiMesh& mesh) -> std::vector<Vertex>;

    Model LoadModel(std::string&& path)
    {
        auto importer = Assimp::Importer{};

        VK_ASSERT(FileExists(path), "File could not be found!");

        static_cast<void>(importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, MAX_BONE_INFLUENCE));
        const auto* scene = importer.ReadFile(path.c_str(), ASSIMP_LOADER_OPTIONS);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            VK_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
            importer.ReadFile("coreAssets/models/error.obj", aiProcess_Triangulate);
        }

        Model model {};

        model.meshes().reserve(scene->mNumMeshes);

        return {};
    }

    auto extract_vertices(const aiMesh& mesh)->std::vector<Vertex>
    {
        std::vector<Vertex> vertices;

        vertices.reserve(mesh.mNumVertices);

        for (unsigned int i = 0; i < mesh.mNumVertices; ++i)
        {
            Vertex vertex {};

            vertex.position = to_glm(mesh.mVertices[i]);
            vertex.normal = to_glm(mesh.mNormals[i]);

            if (mesh.HasTextureCoords(0))
            {
                vertex.uv = to_glm(mesh.mTextureCoords[0][i]);
                vertex.tangent = to_glm(mesh.mTangents[i]);
                vertex.bitangent = to_glm(mesh.mBitangents[i]);
            }



            vertices.push_back(vertex);
        }

        return vertices;
    }
}