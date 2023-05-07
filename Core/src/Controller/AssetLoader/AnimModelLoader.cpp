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

    static glm::vec3 to_glm(aiColor3D& v) { return { v.r, v.g, v.b}; }
    static glm::vec3 to_glm(aiVector3D& v) { return { v.x, v.y, v.z }; }

    std::vector<Mesh> meshes;

    auto extract_vertices(const aiMesh* mesh) -> std::vector<Vertex>;
    auto extract_indices(const aiMesh* mesh)-> std::vector<unsigned int>;
    auto extract_bones(const aiMesh* mesh, std::vector<Vertex>& vertices)->std::pair <std::vector<Bone>, std::unordered_map<std::string, int>>;
    auto extract_textures(const aiMaterial* material, const aiTextureType type)-> std::vector<Texture>;
    auto extract_animations(const aiScene* scene)->std::vector<Animation>;
		
    auto process_node(const aiScene* scene, const aiNode* node)->void;
    auto process_mesh(const aiScene* scene, const aiMesh* assimp_mesh)->Mesh;
    auto process_material(const aiMaterial* material)->MaterialSpec;

    bool IS_CORE_ASSET = false;

    Model LoadAnimatedModel(const std::string& path)
    {
        auto importer = Assimp::Importer{};

        IS_CORE_ASSET = path.find("coreAssets/");

        static_cast<void>(importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.0f));
        VK_ASSERT(FileExists(path), "File could not be found!");

        const auto* scene = importer.ReadFile(path.c_str(), ASSIMP_LOADER_OPTIONS);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            VK_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
            importer.ReadFile("coreAssets/models/error.obj", aiProcess_Triangulate);
        }

        Model model {};

        model.meshes().reserve(scene->mNumMeshes);
        model.animations().reserve(scene->mNumAnimations);

        process_node(scene, scene->mRootNode);

        model.meshes() = meshes;
        model.animations() = extract_animations(scene);

        return model;
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
    	auto vertices = extract_vertices(assimp_mesh);
    	auto indices = extract_indices(assimp_mesh);
        auto [bones, bone_map] = extract_bones(assimp_mesh, vertices);
        auto material = process_material(scene->mMaterials[assimp_mesh->mMaterialIndex]);

        return { vertices, indices, bones, bone_map, material };
    }

    auto process_material(const aiMaterial* material)->MaterialSpec
    {
        std::vector<Texture> textures;

        aiColor3D ambient, diffuse, specular, normal, emission;
        float shininess;

        material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
        material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
        material->Get(AI_MATKEY_COLOR_EMISSIVE, emission);
        material->Get(AI_MATKEY_SHININESS, shininess);

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

    auto extract_vertices(const aiMesh* mesh)-> std::vector<Vertex>
    {
        std::vector<Vertex> vertices;

        vertices.reserve(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex {};

            vertex.position = to_glm(mesh->mVertices[i]);
            vertex.normal = to_glm(mesh->mNormals[i]);

            if (mesh->HasTextureCoords(0))
            {
                vertex.uv = to_glm(mesh->mTextureCoords[0][i]);
                vertex.tangent = to_glm(mesh->mTangents[i]);
                vertex.bitangent = to_glm(mesh->mBitangents[i]);
            }

            std::fill(std::begin(vertex.bone_ids), std::end(vertex.bone_ids), -1);
            std::fill(std::begin(vertex.bone_weights), std::end(vertex.bone_weights), 0.0f);

            vertices.push_back(vertex);
        }

        return vertices;
    }

    auto extract_indices(const aiMesh* mesh) -> std::vector<unsigned int>
    {
        std::vector<unsigned int> indices;

        indices.reserve(static_cast<size_t>(mesh->mNumFaces) * 3);

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            const auto face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }

        return indices;
    }

    auto extract_bones(const aiMesh* mesh, std::vector<Vertex>& vertices) -> std::pair<std::vector<Bone>, std::unordered_map<std::string, int>>
    {
        std::vector<Bone> bones;
        std::unordered_map<std::string, int> bone_map;

        VK_TRACE("Num Bones: {0}", mesh->mNumBones);

        for (unsigned int i = 0; i < mesh->mNumBones; ++i)
        {
            const auto& imported_bone = *mesh->mBones[i];

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

    auto extract_textures(const aiMaterial* material, const aiTextureType type)->std::vector<Texture>
    {
        std::vector<Texture> textures;

        const auto count = material->GetTextureCount(type);

        textures.reserve(count);

        for (unsigned int i = 0; i < count; ++i)
        {
            auto imported_path = aiString{};

        	material->GetTexture(type, i, &imported_path);
            
            auto&& texture = Texture{};

            texture.path = imported_path.C_Str();

            auto final_path = IS_CORE_ASSET ? "assets/" + texture.path : "coreAssets/textures/" + texture.path;
            texture.SetID(LoadTexture(final_path, false, false));

            textures.emplace_back(std::move(texture));
        }

        return textures;
    }

    auto extract_animations(const aiScene* scene)->std::vector<Animation>
    {
        std::vector<Animation> animations;

        VK_TRACE("Num Animations: {0}", scene->mNumAnimations);

        for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
        {
            const auto& imported_animation = *scene->mAnimations[i];

            Animation animation{};

            animation.name = std::string(imported_animation.mName.C_Str());
            animation.frame_rate = static_cast<float>(imported_animation.mTicksPerSecond);
            animation.duration = static_cast<float>(imported_animation.mDuration);

            for (unsigned int j = 0; j < imported_animation.mNumChannels; ++j)
            {
                const auto& imported_animation_channel = *imported_animation.mChannels[j];
            }
        }
    }
}