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

using Vakol::Model::Assets::Mesh;

using Vakol::Model::Assets::Bone;

using Vakol::Model::MAX_BONE_INFLUENCE;

namespace Vakol::Controller
{
    constexpr unsigned int ASSIMP_LOADER_OPTIONS =
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights;

    static glm::mat4 to_glm(const aiMatrix4x4& m)
    {
    	glm::vec4 c1{ m.a1, m.b1, m.c1, m.d1 };
    	glm::vec4 c2{ m.a2, m.b2, m.c2, m.d2 };
    	glm::vec4 c3{ m.a3, m.b3, m.c3, m.d3 };
    	glm::vec4 c4{ m.a4, m.b4, m.c4, m.d4 };

        return { c1, c2, c3, c4 };
    }

    static glm::quat to_glm(const aiQuaternion& q) { return { q.w, q.x, q.y, q.z }; }

    static glm::vec3 to_glm(const aiVector3D& v) { return { v.x, v.y, v.z }; }

    std::vector<Mesh> meshes;

    auto extract_vertices(const aiMesh& mesh) -> std::vector<Vertex>;
    auto extract_indices(const aiMesh& mesh)-> std::vector<unsigned int>;
    auto extract_bones(const aiMesh& mesh)->std::pair <std::vector<Bone>, std::unordered_map<std::string, int>>;

    auto process_node(const aiScene* scene, const aiNode* node)->void;
    auto process_mesh(const aiScene* scene, const aiMesh* assimp_mesh)->Mesh;


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

    // recursively iterate through each node for meshes
    auto process_node(const aiScene* scene, const aiNode* node)->void
    {
        // Fetch meshes in current node
	    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	    {
            const auto* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(process_mesh(scene, mesh));
	    }

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
            process_node(scene, node->mChildren[i]);
    }

    auto process_mesh(const aiScene* scene, const aiMesh* assimp_mesh)->Mesh
    {
        Mesh mesh {};

        return mesh;
    }

    auto extract_vertices(const aiMesh& mesh)-> std::vector<Vertex>
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

            std::fill(std::begin(vertex.bone_ids), std::end(vertex.bone_ids), -1);
            std::fill(std::begin(vertex.bone_weights), std::end(vertex.bone_weights), 0.0f);

            vertices.push_back(vertex);
        }

        return vertices;
    }

    auto extract_indices(const aiMesh& mesh) -> std::vector<unsigned int>
    {
        std::vector<unsigned int> indices;

        indices.reserve(static_cast<size_t>(mesh.mNumFaces) * 3);

        for (unsigned int i = 0; i < mesh.mNumFaces; ++i)
        {
            const auto face = mesh.mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }

        return indices;
    }

    auto extract_bones(const aiMesh& mesh, std::vector<Vertex>& vertices) -> std::pair<std::vector<Bone>, std::unordered_map<std::string, int>>
    {
        std::vector<Bone> bones;
        std::unordered_map<std::string, int> bone_map;

        for (unsigned int i = 0; i < mesh.mNumBones; ++i)
        {
            const auto& imported_bone = *mesh.mBones[i];

        	auto name = std::string { imported_bone.mName.data };

            if (bone_map.find(name) == bone_map.end())
            {
                auto bone = Bone{};

            	bone.name = name;
                bone.index = static_cast<int>(bone_map.size());
                bone.offset = to_glm(imported_bone.mOffsetMatrix); // inverse-bind pose matrix

                bone_map[bone.name] = bone.index;

                bones.push_back(std::move(bone));
            }

            const auto bone_index = bone_map.at(name);

            for (unsigned int j = 0; j < imported_bone.mNumWeights; ++j)
            {
                const auto& imported_weight = imported_bone.mWeights[j];
                const auto index = static_cast<int>(imported_weight.mVertexId);
                const auto bone_weight = imported_weight.mWeight;

                std::fill(std::begin(vertices[index].bone_ids), std::end(vertices[index].bone_ids), bone_index);
                std::fill(std::begin(vertices[index].bone_weights), std::end(vertices[index].bone_weights), bone_weight);
            }
        }

        return std::make_pair(bones, bone_map);
    }
}