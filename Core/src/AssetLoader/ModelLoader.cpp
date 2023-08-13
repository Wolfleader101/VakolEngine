#include "AssetLoader/ModelLoader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <stack>

#include "AssetLoader/AssetLoader.hpp"
#include "AssetLoader/FileLoader.hpp"
#include "AssetLoader/TextureLoader.hpp"
#include "Logger/Logger.hpp"
#include "Math/Math.hpp"
#include "Rendering/Assets/Animation/Animation.hpp"
#include "Rendering/Assets/Animation/Keyframe.hpp"

using Vakol::Vertex;

using namespace Vakol::Assets;
using namespace Vakol::Math;

namespace Vakol
{
    constexpr unsigned int ASSIMP_LOADER_OPTIONS = aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                                   aiProcess_CalcTangentSpace | aiProcess_FlipUVs |
                                                   aiProcess_LimitBoneWeights | aiProcess_GlobalScale;

    static Mat4 to_glm(const aiMatrix4x4& m)
    {
        Vec4 c1{m.a1, m.b1, m.c1, m.d1};
        Vec4 c2{m.a2, m.b2, m.c2, m.d2};
        Vec4 c3{m.a3, m.b3, m.c3, m.d3};
        Vec4 c4{m.a4, m.b4, m.c4, m.d4};

        return {c1, c2, c3, c4};
    }

    static Quat to_glm(aiQuaternion& v)
    {
        return {v.w, v.x, v.y, v.z};
    }
    static Vec3 to_glm(aiColor3D& v)
    {
        return {v.r, v.g, v.b};
    }
    static Vec3 to_glm(aiVector3D& v)
    {
        return {v.x, v.y, v.z};
    }

    auto extract_vertices(const aiMesh& mesh) -> std::vector<Vertex>;
    auto extract_indices(const aiMesh& mesh) -> std::vector<unsigned int>;
    auto extract_bones(const aiMesh& mesh, std::vector<Vertex>& vertices, BoneMap& bone_map) -> void;
    auto extract_textures(const aiScene& scene, const aiMaterial* material, aiTextureType type) -> std::vector<Texture>;

    auto extract_keyframes(const aiNodeAnim& channel, const Bone& bone_info) -> KeyFrame;
    auto extract_animations(const aiScene& scene, const BoneMap& bone_map) -> std::vector<Animation>;

    auto process_meshes(const aiScene& scene, BoneMap& bone_map) -> std::vector<Mesh>;
    auto process_mesh(const aiScene& scene, const aiMesh& mesh, BoneMap& bone_map) -> Mesh;
    auto process_material(const aiScene& scene, const aiMaterial* material) -> MaterialSpec;

    bool IS_CORE_ASSET = false;

    std::pair<::Model, Animator> LoadModel(const std::string& path, const float scale, bool animated)
    {
        auto importer = Assimp::Importer{};

        IS_CORE_ASSET = path.find("coreAssets/");

        static_cast<void>(importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, scale));

        if (!FileExists(path))
        {
            VK_CRITICAL("File could not be found!");
        }

        auto* scene = importer.ReadFile(path.c_str(), ASSIMP_LOADER_OPTIONS);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            VK_CRITICAL("ASSIMP:: {0}", importer.GetErrorString());

            scene = importer.ReadFile("coreAssets/models/error.obj", aiProcess_Triangulate);
            VK_CRITICAL("Using Default ERROR model");
        }

        VK_TRACE("Model Path: {0}", path);
        VK_TRACE("Animations Found: {0}", scene->mNumAnimations);

        for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
            VK_TRACE("ANIMATION | NAME: {0} | STATE: {1}", scene->mAnimations[i]->mName.C_Str(), i);

        std::cout << std::endl;

        BoneMap bone_map;

        auto meshes = process_meshes(*scene, bone_map);

        std::vector<Animation> animations{};

        if (animated && scene->mNumAnimations > 0)
            animations = extract_animations(*scene, bone_map);

        return std::make_pair(::Model(meshes), Animator(animations));
    }

    // iteratively iterate through each node for meshes
    auto process_meshes(const aiScene& scene, BoneMap& bone_map) -> std::vector<Mesh>
    {
        std::vector<Mesh> meshes;

        // Fetch meshes in current node
        for (unsigned int i = 0; i < scene.mNumMeshes; ++i)
        {
            const auto& mesh = *scene.mMeshes[i];

            // VK_TRACE("Mesh - NAME: {0} | NUMBER OF BONES: {2}", mesh.mName.C_Str(), mesh.mNumBones);

            meshes.push_back(process_mesh(scene, mesh, bone_map));
        }

        return meshes;
    }

    auto process_mesh(const aiScene& scene, const aiMesh& mesh, BoneMap& bone_map) -> Mesh
    {
        auto vertices = extract_vertices(mesh);
        auto indices = extract_indices(mesh);

        if (mesh.HasBones())
            extract_bones(mesh, vertices, bone_map);

        auto material = process_material(scene, scene.mMaterials[mesh.mMaterialIndex]);

        return {vertices, indices, sizeof(Vertex), material};
    }

    auto process_material(const aiScene& scene, const aiMaterial* material) -> MaterialSpec
    {
        std::vector<Texture> textures;

        aiColor3D ambient, diffuse, specular, emission;
        float shininess;

        material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
        material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
        material->Get(AI_MATKEY_COLOR_EMISSIVE, emission);
        material->Get(AI_MATKEY_SHININESS, shininess);

        auto diffuse_maps = extract_textures(scene, material, aiTextureType_DIFFUSE);
        auto specular_maps = extract_textures(scene, material, aiTextureType_SPECULAR);
        auto normal_maps = extract_textures(scene, material, aiTextureType_NORMALS);
        auto emission_maps = extract_textures(scene, material, aiTextureType_EMISSIVE);

        textures.insert(textures.end(), std::make_move_iterator(diffuse_maps.begin()),
                        std::make_move_iterator(diffuse_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(specular_maps.begin()),
                        std::make_move_iterator(specular_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(normal_maps.begin()),
                        std::make_move_iterator(normal_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(emission_maps.begin()),
                        std::make_move_iterator(emission_maps.end()));

        return {to_glm(ambient), to_glm(diffuse), to_glm(specular), to_glm(emission), shininess, std::move(textures)};
    }

    auto extract_vertices(const aiMesh& mesh) -> std::vector<Vertex>
    {
        std::vector<Vertex> vertices;

        vertices.reserve(mesh.mNumVertices);

        for (unsigned int i = 0; i < mesh.mNumVertices; ++i)
        {
            Vertex vertex{};

            vertex.position = to_glm(mesh.mVertices[i]);
            vertex.normal = to_glm(mesh.mNormals[i]);

            if (mesh.HasTextureCoords(0))
            {
                vertex.uv = to_glm(mesh.mTextureCoords[0][i]);
                if (mesh.HasTangentsAndBitangents())
                {
                    vertex.tangent = to_glm(mesh.mTangents[i]);
                    vertex.bitangent = to_glm(mesh.mBitangents[i]);
                }
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

    auto extract_bones(const aiMesh& mesh, std::vector<Vertex>& vertices, BoneMap& bone_map) -> void
    {
        auto add_bone_weight_to_vertex = [](Vertex& vertex, const int bone_index, const float bone_weight) {
            // There should only be up to 4 bone influences supported.
            const auto itr = std::find_if(std::begin(vertex.bone_ids), std::end(vertex.bone_ids),
                                          [&](const int index) { return index < 0 && index != bone_index; });

            if (itr == std::end(vertex.bone_ids))
                return;

            const size_t i =
                std::distance(std::begin(vertex.bone_ids), itr); // Number of elements between two iterators

            vertex.bone_ids[i] = bone_index;
            vertex.bone_weights[i] = bone_weight;
        };

        for (unsigned int i = 0; i < mesh.mNumBones; ++i)
        {
            const aiString& bone_name = mesh.mBones[i]->mName;

            const auto bone_index = bone_map.add_new_bone(std::string(bone_name.data, bone_name.length),
                                                          to_glm(mesh.mBones[i]->mOffsetMatrix));

            const aiBone* const bone = mesh.mBones[i];

            if (bone == nullptr)
            {
                VK_CRITICAL("extract_bones - Bone {0} is null", bone_name.C_Str());
                continue;
            }

            const aiVertexWeight* const weights = bone->mWeights;

            for (unsigned int j = 0; j < bone->mNumWeights; ++j)
            {
                const auto vertex_id = weights[j].mVertexId;
                const auto bone_weight = weights[j].mWeight;

                if (vertex_id > static_cast<unsigned int>(vertices.size()))
                {
                    VK_CRITICAL("extract_bones - Vertex ID must not be greater than the number of vertices!");
                    continue;
                }

                add_bone_weight_to_vertex(vertices[vertex_id], bone_index, bone_weight);
            }
        }
    }

    auto extract_textures(const aiScene& scene, const aiMaterial* material, const aiTextureType type)
        -> std::vector<Texture>
    {
        std::vector<Texture> textures;

        const auto count = material->GetTextureCount(type);

        textures.reserve(count);

        for (unsigned int i = 0; i < count; ++i)
        {
            auto imported_path = aiString{};
            auto&& texture = Texture{};

            if (material->GetTexture(type, i, &imported_path) == AI_SUCCESS)
            {
                if (const auto embedded_texture = scene.GetEmbeddedTexture(imported_path.C_Str()))
                {
                    const auto size = embedded_texture->mWidth;

                    texture = *AssetLoader::GetTexture(embedded_texture->mFilename.C_Str(), static_cast<int>(size),
                                                       false, false, embedded_texture->pcData);
                }
                else
                {
                    texture.path = imported_path.C_Str();
                    auto final_path = IS_CORE_ASSET ? "assets/" + texture.path : "coreAssets/textures/" + texture.path;
                    texture = *AssetLoader::GetTexture(final_path, false, false);
                }
            }

            textures.push_back(std::move(texture));
        }

        return textures;
    }

    auto extract_keyframes(const aiNodeAnim& channel, const Bone& bone_info) -> KeyFrame
    {
        KeyFrame key_frame;

        // Set the index and offset from the provided BoneInfo
        key_frame.index = bone_info.index;
        key_frame.offset = bone_info.offset;

        // Extract position keyframes
        key_frame.positions.reserve(channel.mNumPositionKeys);
        for (unsigned int index = 0; index < channel.mNumPositionKeys; ++index)
        {
            KeyPosition data;

            data.position = to_glm(channel.mPositionKeys[index].mValue);
            data.timestamp = static_cast<float>(channel.mPositionKeys[index].mTime);

            key_frame.positions.push_back(data);
        }

        // Extract rotation keyframes
        key_frame.rotations.reserve(channel.mNumRotationKeys);
        for (unsigned int index = 0; index < channel.mNumRotationKeys; ++index)
        {
            KeyRotation data;

            data.rotation = to_glm(channel.mRotationKeys[index].mValue);
            data.timestamp = static_cast<float>(channel.mRotationKeys[index].mTime);

            key_frame.rotations.push_back(data);
        }

        // Extract scale keyframes
        key_frame.scales.reserve(channel.mNumScalingKeys);
        for (unsigned int index = 0; index < channel.mNumScalingKeys; ++index)
        {
            KeyScale data;

            data.scale = to_glm(channel.mScalingKeys[index].mValue);
            data.timestamp = static_cast<float>(channel.mScalingKeys[index].mTime);

            key_frame.scales.push_back(data);
        }

        return key_frame;
    }

    auto extract_animations(const aiScene& scene, const BoneMap& bone_map) -> std::vector<Animation>
    {
        std::vector<Animation> animations;

        for (unsigned int animation_index = 0; animation_index < scene.mNumAnimations; ++animation_index)
        {
            // Retrieve the animation from the scene
            const aiAnimation* const animation = scene.mAnimations[animation_index];

            // Convert the animation duration and ticks per second to float
            const auto duration = static_cast<float>(animation->mDuration);
            const auto ticks_per_second = static_cast<float>(animation->mTicksPerSecond);

            // Containers for storing animation node data and node names
            std::vector<AnimationNode> nodes;
            std::vector<const aiString*> node_names;

            // Struct for representing a node during depth-first search traversal
            struct Node
            {
                const aiNode* src = nullptr; // Source node
                int parent = -1;             // Index of the parent node in the 'nodes' vector
            };

            std::stack<Node> node_search;
            node_search.push(Node{scene.mRootNode, -1}); // Push the root node with no associated parent node (-1)

            // Depth-first search traversal to extract animation node data
            while (!node_search.empty())
            {
                const auto [src, parent] = node_search.top();
                node_search.pop();

                AnimationNode node;
                node.parent = parent;
                node.node_transform = to_glm(src->mTransformation);

                // Ensure that the parent index is within the valid range
                if (node.parent >= static_cast<int>(nodes.size()))
                {
                    VK_CRITICAL("extract_animations - Parent index out of valid range");
                    continue;
                }

                // Store the current node and its name
                nodes.push_back(node);
                node_names.push_back(&src->mName);

                const auto parent_index = static_cast<int>(nodes.size() - 1);

                // Add the children of the current node to the search stack
                for (unsigned int i = 0; i < src->mNumChildren; ++i)
                {
                    if (src->mNumChildren < 1)
                        continue;
                    node_search.push(Node{src->mChildren[i], parent_index});
                }
            }

            // Extract bone keyframes for each channel in the animation
            for (unsigned int i = 0; i < animation->mNumChannels; ++i)
            {
                const auto channel = animation->mChannels[i];

                const aiString& bone_name = channel->mNodeName;

                // Find the node with a matching name in the 'node_names' vector
                auto itr = std::find_if(node_names.cbegin(), node_names.cend(),
                                        [&bone_name](const aiString* node_name) { return bone_name == *node_name; });

                // Ensure that a matching node is found
                if (itr == node_names.end())
                {
                    VK_CRITICAL("extract_animations - No node matching a bone.");
                    continue;
                }

                // Calculate the index of the node in the 'nodes' vector
                const auto index = static_cast<int>(std::distance(node_names.cbegin(), itr));

                // Blender includes an armature node *we don't want that!*
                if (strcmp(bone_name.C_Str(), "Armature") == 0)
                    continue;

                // Retrieve the bone info based on the bone name
                const Bone* info = bone_map.get(bone_name.C_Str());

                if (!info)
                    continue;

                auto& [bone, bone_transform, parent, node_transform] = nodes[index];

                if (bone.has_value())
                {
                    VK_CRITICAL("extract_animations - Two or more bones matching same node.");
                    continue;
                }

                bone.emplace(extract_keyframes(*channel, *info));
            }

            const auto& root_transform = to_glm(scene.mRootNode->mTransformation);
            const auto bone_count = static_cast<unsigned int>(bone_map.name_to_info.size());

            animations.emplace_back(inverse(root_transform), std::move(nodes), bone_count, duration, ticks_per_second);
        }

        return animations;
    }
} // namespace Vakol