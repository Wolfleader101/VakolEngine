#include "ModelLoader.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>
#include <Controller/AssetLoader/TextureLoader.hpp>

#include <stack>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/quaternion.hpp>
#pragma warning(pop)

#include <Controller/Logger.hpp>

using Vakol::Model::Vertex;

using namespace Vakol::Model::Assets;

namespace Vakol::Controller
{
    constexpr unsigned int ASSIMP_LOADER_OPTIONS =
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_LimitBoneWeights | aiProcess_GlobalScale;

    static glm::mat4 to_glm(const aiMatrix4x4& m)
    {
    	glm::vec4 c1{ m.a1, m.b1, m.c1, m.d1 };
    	glm::vec4 c2{ m.a2, m.b2, m.c2, m.d2 };
    	glm::vec4 c3{ m.a3, m.b3, m.c3, m.d3 };
    	glm::vec4 c4{ m.a4, m.b4, m.c4, m.d4 };

        return { c1, c2, c3, c4 };
    }

    static glm::quat to_glm(aiQuaternion& v) { return { v.w, v.x, v.y, v.z }; }
    static glm::vec3 to_glm(aiColor3D& v) { return { v.r, v.g, v.b}; }
    static glm::vec3 to_glm(aiVector3D& v) { return { v.x, v.y, v.z }; }

    auto extract_vertices(const aiMesh& mesh) -> std::vector<Vertex>;
    auto extract_indices(const aiMesh& mesh)-> std::vector<unsigned int>;
    auto extract_bones(const aiMesh& mesh, std::vector<Vertex>& vertices, BoneInfoRemap& bone_info)->void;
    auto extract_textures(const aiMaterial& material, aiTextureType type)->std::vector<Texture>;
    auto extract_bone_keyframes(const aiNodeAnim& channel, const BoneInfo& bone_info)->BoneKeyFrame;
    auto extract_animation(const aiScene& scene, int animation_index, const BoneInfoRemap& bone_info)->Animation;
		
    auto process_meshes(const aiScene& scene, BoneInfoRemap& bone_info)->std::vector<Mesh>;
    auto process_mesh(const aiScene& scene, const aiMesh& assimp_mesh, BoneInfoRemap& bone_info)->Mesh;
    auto process_material(const aiMaterial& material)->MaterialSpec;

    bool IS_CORE_ASSET = false;

    ::Model LoadModel(const std::string& path)
    {
        auto importer = Assimp::Importer{};

        IS_CORE_ASSET = path.find("coreAssets/");

        static_cast<void>(importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 0.01f));
        VK_ASSERT(FileExists(path), "File could not be found!");

        const auto* scene = importer.ReadFile(path.c_str(), ASSIMP_LOADER_OPTIONS);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            VK_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
            importer.ReadFile("coreAssets/models/error.obj", aiProcess_Triangulate);
        }

        BoneInfoRemap bone_info;

        //auto meshes = 
        //auto animation = 

        return { process_meshes(*scene, bone_info) , extract_animation(*scene, -1, bone_info) };
    }

    // iteratively iterate through each node for meshes
    auto process_meshes(const aiScene& scene, BoneInfoRemap& bone_info)->std::vector<Mesh>
    {
        std::vector<Mesh> meshes;

        std::stack<const aiNode*> nodes;

        nodes.push(scene.mRootNode);

        while(!nodes.empty())
        {
            const auto& node = *nodes.top(); // current node
            nodes.pop();

            // Fetch meshes in current node
            for (unsigned int i = 0; i < node.mNumMeshes; ++i)
            {
                const auto& mesh = *scene.mMeshes[node.mMeshes[i]];
                
                meshes.push_back(process_mesh(scene, mesh, bone_info));
            }

            for (unsigned int i = 0; i < node.mNumChildren; ++i)
                nodes.push(node.mChildren[i]);
        }

        return meshes;
    }

    auto process_mesh(const aiScene& scene, const aiMesh& assimp_mesh, BoneInfoRemap& bone_info)->Mesh
    {
    	auto vertices = extract_vertices(assimp_mesh);
    	auto indices = extract_indices(assimp_mesh);
        extract_bones(assimp_mesh, vertices, bone_info);
        auto material = process_material(*scene.mMaterials[assimp_mesh.mMaterialIndex]);

        return { vertices, indices, sizeof(Vertex), material };
    }

    auto process_material(const aiMaterial& material)->MaterialSpec
    {
        std::vector<Texture> textures;

        aiColor3D ambient, diffuse, specular, normal, emission;
        float shininess;

        material.Get(AI_MATKEY_COLOR_AMBIENT, ambient);
        material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
        material.Get(AI_MATKEY_COLOR_SPECULAR, specular);
        material.Get(AI_MATKEY_COLOR_EMISSIVE, emission);
        material.Get(AI_MATKEY_SHININESS, shininess);

        auto diffuse_maps = extract_textures(material, aiTextureType_DIFFUSE);
        auto specular_maps = extract_textures(material, aiTextureType_SPECULAR);
        auto normal_maps = extract_textures(material, aiTextureType_NORMALS);
        auto emission_maps = extract_textures(material, aiTextureType_EMISSIVE);

        textures.insert(textures.end(), std::make_move_iterator(diffuse_maps.begin()), std::make_move_iterator(diffuse_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(specular_maps.begin()), std::make_move_iterator(specular_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(normal_maps.begin()), std::make_move_iterator(normal_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(emission_maps.begin()), std::make_move_iterator(emission_maps.end()));

        return {to_glm(ambient), to_glm(diffuse), to_glm(specular), to_glm(emission), shininess, std::move(textures) };
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

    auto extract_bones(const aiMesh& mesh, std::vector<Vertex>& vertices, BoneInfoRemap& bone_info)->void
    {
        auto add_bone_weight_to_vertex = [](Vertex& vertex, const int bone_index, const float bone_weight)
        {
            // There should only be up to 4 bone influences supported.
            const auto itr = std::find_if(std::begin(vertex.bone_ids), std::end(vertex.bone_ids), [&](const int index)
            {
	            return index < 0 && index != bone_index; // if index = -1 (initial index) AND ...
            });

            VK_ASSERT(itr != std::end(vertex.bone_ids), "\n\nEither more than 4 bones per vertex OR duplicated bone.");

            const size_t i = std::distance(std::begin(vertex.bone_ids), itr); // Number of elements between two iterators

            vertex.bone_ids[i] = bone_index;
            vertex.bone_weights[i] = bone_weight;
        };

        VK_TRACE(mesh.mNumBones);

        for (unsigned int i = 0; i < mesh.mNumBones; ++i)
        {
            const aiString& bone_name = mesh.mBones[i]->mName;

            const auto bone_index = bone_info.add_new_bone(std::string(bone_name.data, bone_name.length), to_glm(mesh.mBones[i]->mOffsetMatrix));

            const aiBone* const bone = mesh.mBones[i];

            VK_ASSERT(bone, "Bone is nullptr");

            const aiVertexWeight* const weights = bone->mWeights;

            for (unsigned int j = 0; j < bone->mNumWeights; ++j)
            {
                const auto vertex_id = weights[j].mVertexId;
                const auto bone_weight = weights[j].mWeight;

                VK_ASSERT(vertex_id <= static_cast<unsigned int>(vertices.size()), "id must not be greater than number of vertices!");

                add_bone_weight_to_vertex(vertices[vertex_id], bone_index, bone_weight);
            }
        }
    }

    auto extract_textures(const aiMaterial& material, const aiTextureType type)->std::vector<Texture>
    {
        std::vector<Texture> textures;

        const auto count = material.GetTextureCount(type);

        textures.reserve(count);

        for (unsigned int i = 0; i < count; ++i)
        {
            auto imported_path = aiString{};

        	material.GetTexture(type, i, &imported_path);
            
            auto&& texture = Texture{};

            texture.path = imported_path.C_Str();

            auto final_path = IS_CORE_ASSET ? "assets/" + texture.path : "coreAssets/textures/" + texture.path;
            texture.SetID(LoadTexture(final_path, false, false));

            textures.push_back(std::move(texture));
        }

        return textures;
    }

    auto extract_bone_keyframes(const aiNodeAnim& channel, const BoneInfo& bone_info)->BoneKeyFrame
    {
        BoneKeyFrame bone;

        bone.bone_index = bone_info.index;
        bone.offset = bone_info.offset;

        bone.positions.reserve(channel.mNumPositionKeys);
        for (unsigned int index = 0; index < channel.mNumPositionKeys; ++index)
        {
            KeyPosition data;

            data.position = to_glm(channel.mPositionKeys[index].mValue);
            data.timestamp = static_cast<float>(channel.mPositionKeys[index].mTime);

        	bone.positions.push_back(data);
        }

        bone.scales.reserve(channel.mNumScalingKeys);
        for (unsigned int index = 0; index < channel.mNumScalingKeys; ++index)
        {
            KeyScale data;

            data.scale = to_glm(channel.mPositionKeys[index].mValue);
            data.timestamp = static_cast<float>(channel.mScalingKeys[index].mTime);

            bone.scales.push_back(data);
        }

        bone.rotations.reserve(channel.mNumRotationKeys);
        for (unsigned int index = 0; index < channel.mNumRotationKeys; ++index)
        {
            KeyRotation data;

            data.rotation = to_glm(channel.mRotationKeys[index].mValue);
            data.timestamp = static_cast<float>(channel.mRotationKeys[index].mTime);

            bone.rotations.push_back(data);
        }

        return bone;
    }

    auto extract_animation(const aiScene& scene, int animation_index, const BoneInfoRemap& bone_info)->Animation
    {
        if (scene.mNumAnimations == 0)
        {
            VK_WARN("No Animations Found!");
            return {};
        }

        animation_index = std::max(0, animation_index);

        VK_ASSERT(static_cast<unsigned int>(animation_index) < scene.mNumAnimations, "No");

        const aiAnimation* const animation = scene.mAnimations[animation_index];

        const auto duration = static_cast<float>(animation->mDuration);
        const auto ticks_per_second = static_cast<float>(animation->mTicksPerSecond);

        std::vector<AnimNode> nodes;
        std::vector<const aiString*> node_names;

        struct Node
        {
            const aiNode* src = nullptr;
            int parent = -1;
        };

        std::stack<Node> node_search;
        node_search.push(Node{ scene.mRootNode, -1 }); // -1 represents no associated parent node

        while (!node_search.empty())
        {
	        const auto [src, parent] = node_search.top();
            node_search.pop();

            AnimNode node;
            node.parent = parent;
            node.node_transform = to_glm(src->mTransformation);

            VK_ASSERT(node.parent < static_cast<int>(nodes.size()), "doin it wrong");

            nodes.push_back(node);
            node_names.push_back(&src->mName);

            const auto parent_index = static_cast<int>(nodes.size() - 1);

            for (unsigned int i = 0; i < src->mNumChildren; ++i)
                node_search.push(Node{ src->mChildren[i], parent_index });
        }

        for (unsigned int i = 0; i < animation->mNumChannels; ++i)
        {
	        const auto channel = animation->mChannels[i];

            const aiString& bone_name = channel->mNodeName;

            auto itr = std::find_if(node_names.cbegin(), node_names.cend(), [&bone_name](const aiString* node_name)
            {
                return bone_name == *node_name;
            });

            VK_ASSERT(itr != node_names.end(), "\n\nNo node matching a bone.");

            const auto index = static_cast<int>(std::distance(node_names.cbegin(), itr));

            const BoneInfo* info = bone_info.get(bone_name.C_Str());

            VK_ASSERT(info, "\n\nNo bone info remap matching a bone");

            auto& [bone, bone_transform, parent, node_transform] = nodes[index];

            VK_ASSERT(!bone.has_value(), "\n\nTwo or more bones matching same node.");

            bone.emplace(extract_bone_keyframes(*channel, *info));
        }

        const auto& root = to_glm(scene.mRootNode->mTransformation);
        const auto bone_count = static_cast<unsigned int>(bone_info.name_to_info.size());

        return { inverse(root), std::move(nodes), bone_count, duration, ticks_per_second };
    }
}