#include "ModelImporter.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Controller/AssetLoader/FileLoader.hpp"
#include "Controller/Logger.hpp"

#include "../Material.hpp"
#include "../Animation.hpp"
#include "../Texture.hpp"

#include "Rendering/RenderData.hpp"

#include "Math/Math.hpp"
#include "TextureImporter.hpp"
#include "Rendering/RenderAPI.hpp"
#include "Rendering/Platform/OpenGL/Texture.hpp"

#include <iostream>
#include <stack>

using namespace Vakol::Rendering::Assets;

constexpr int ASSIMP_LOADER_OPTIONS =
aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes | aiProcess_JoinIdenticalVertices |
aiProcess_ImproveCacheLocality | aiProcess_SplitLargeMeshes | aiProcess_ValidateDataStructure |
aiProcess_FindInvalidData | aiProcess_GlobalScale | aiProcess_PopulateArmatureData | aiProcess_FlipUVs;


namespace Vakol::Rendering::Assets::Importer
{
    /*Helper Functions*/
    static void Mat4(const aiMatrix4x4& in, Math::Mat4& out);
    static void Quaternion(const aiQuaternion& in, Math::Quaternion& out);
    static void Vec3(const aiVector3D& in, Math::Vec3& out);
    static void Vec3(const aiColor3D& in, Math::Vec3& out);
    static void Vec2(const aiVector3D& in, Math::Vec2& out);

    static Math::Vec3 ToGLM(const aiVector3D& v);
    static Math::Vec3 ToGLM(const aiColor3D& v);

    /*Mesh Internals*/
    static void ExtractVertices(aiMesh* const& in, Mesh& mesh);

    static void ExtractBones(unsigned int count, aiBone** const& in, std::vector<Bone>& bones);
    static void ProcessBone(aiBone* const& in, Bone& bone);

    /*Animations*/
    static void ExtractAnimations(unsigned int count, aiAnimation** const& in);
    static void ProcessAnimation(aiAnimation* const& in, Animation& animation);

    static void ExtractChannels(unsigned int count, aiNodeAnim** const& in, std::vector<Channel>& channels);
    static void ProcessChannel(aiNodeAnim* const& in, Channel& channel);

    /*Old Importer*/

    static void extract_meshes(const aiScene& scene, std::vector<Mesh>& meshes);
    static Mesh process_mesh(const aiScene& scene, const aiMesh& mesh);

    static Material process_material(const aiScene& scene, const aiMaterial* material);

    static std::vector<Vertex> extract_vertices(const aiMesh& mesh);
    static std::vector<unsigned int> extract_indices(const aiMesh& mesh);

    static std::vector<Texture> extract_textures(const aiScene& scene, const aiMaterial* material, aiTextureType type);

    Model ImportModel(const char* path, const float scale, bool& success)
    {
        auto importer = Assimp::Importer{};

        if (!FileExists(path)) VK_WARN("Model at path {0} was not found!", path);

        static_cast<void>(importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, scale));
        auto* scene = importer.ReadFile(path, ASSIMP_LOADER_OPTIONS);

        Model model;

        // if no scene flags were set (loader options) or the imported scene or root node is nullptr.
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            VK_ERROR("ASSIMP ERROR: {0}", importer.GetErrorString());

            success = false;

            return model;
        }

        std::cout << std::endl;

        VK_TRACE("Stats for Model: {0}", path);

        extract_meshes(*scene, model.meshes);

        std::cout << std::endl;

        success = true;

        return model;
    }

    void extract_meshes(const aiScene& scene, std::vector<Mesh>& meshes)
    {
        // Fetch meshes in current node
        for (unsigned int i = 0; i < scene.mNumMeshes; ++i)
        {
            const auto& mesh = *scene.mMeshes[i];

            meshes.emplace_back(process_mesh(scene, mesh));
        }
    }

    Mesh process_mesh(const aiScene& scene, const aiMesh& mesh)
    {
        const auto& vertices = extract_vertices(mesh);
        const auto& indices = extract_indices(mesh);

        const auto& material = process_material(scene, scene.mMaterials[mesh.mMaterialIndex]);

        return {mesh.mName.C_Str(), vertices, indices, std::vector<Bone>(), std::make_shared<Material>(material)};
    }

    std::vector<Vertex> extract_vertices(const aiMesh& mesh)
    {
        std::vector<Vertex> vertices;

        vertices.reserve(mesh.mNumVertices);

        for (unsigned int i = 0; i < mesh.mNumVertices; ++i)
        {
            Vertex vertex{};

            vertex.position = ToGLM(mesh.mVertices[i]);
            vertex.normal = ToGLM(mesh.mNormals[i]);

            if (mesh.HasTextureCoords(0))
            {
                vertex.uv = ToGLM(mesh.mTextureCoords[0][i]);
                if (mesh.HasTangentsAndBitangents())
                {
                    vertex.tangent = ToGLM(mesh.mTangents[i]);
                    vertex.bitangent = ToGLM(mesh.mBitangents[i]);
                }
            }

            std::fill(std::begin(vertex.boneIDs), std::end(vertex.boneIDs), -1);
            std::fill(std::begin(vertex.boneWeights), std::end(vertex.boneWeights), 0.0f);

            vertices.emplace_back(vertex);
        }

        return vertices;
    }

    std::vector<unsigned int> extract_indices(const aiMesh& mesh)
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

    std::vector<Texture> extract_textures(const aiScene& scene, const aiMaterial* material, const aiTextureType type)
    {
        std::vector<Texture> textures;

        const auto count = material->GetTextureCount(type);

        textures.reserve(count);

        unsigned char* pixels = nullptr;

        for (unsigned int i = 0; i < count; ++i)
        {
            auto imported_path = aiString{};
            auto&& texture = Texture{};

            if (material->GetTexture(type, i, &imported_path) == AI_SUCCESS)
            {
                texture.path = imported_path.C_Str();
                texture.type = static_cast<VK_TEXTURE_TYPE>(type);

                if (const auto& embedTexture = scene.GetEmbeddedTexture(imported_path.C_Str()))
                {
                    texture.embedded = true;

                    ImportTexture(embedTexture->pcData, static_cast<int>(embedTexture->mWidth), texture.width, texture.height, texture.channels, pixels);

                    texture.ID = OpenGL::GenerateTexture(texture.width, texture.height, texture.channels, pixels);
                }

                textures.emplace_back(std::move(texture));
            }
        }

        return textures;
    }

    Material process_material(const aiScene& scene, const aiMaterial* material)
    {
        std::vector<Texture> textures;

        MaterialProperties properties;

        material->Get(AI_MATKEY_COLOR_AMBIENT, properties.ambient_color);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, properties.diffuse_color);
        material->Get(AI_MATKEY_COLOR_SPECULAR, properties.specular_color);
        material->Get(AI_MATKEY_COLOR_EMISSIVE, properties.emissive_color);
        material->Get(AI_MATKEY_SHININESS, properties.shininess);
        material->Get(AI_MATKEY_EMISSIVE_INTENSITY, properties.intensity);
        material->Get(AI_MATKEY_ENABLE_WIREFRAME, properties.wireframe);

        auto&& diffuse_maps = extract_textures(scene, material,  aiTextureType_DIFFUSE);
        auto&& specular_maps = extract_textures(scene, material, aiTextureType_SPECULAR);
        auto&& ambient_maps = extract_textures(scene, material,  aiTextureType_AMBIENT);
        auto&& height_maps = extract_textures(scene, material,   aiTextureType_HEIGHT);
        auto&& emission_maps = extract_textures(scene, material, aiTextureType_EMISSIVE);
        auto&& normal_maps = extract_textures(scene, material,   aiTextureType_NORMALS);

        textures.insert(textures.end(), std::make_move_iterator(diffuse_maps.begin()),
                        std::make_move_iterator(diffuse_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(specular_maps.begin()),
                        std::make_move_iterator(specular_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(ambient_maps.begin()),
                        std::make_move_iterator(ambient_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(height_maps.begin()),
                        std::make_move_iterator(height_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(emission_maps.begin()),
                        std::make_move_iterator(emission_maps.end()));
        textures.insert(textures.end(), std::make_move_iterator(normal_maps.begin()),
                        std::make_move_iterator(normal_maps.end()));

        return {material->GetName().C_Str(), "null", "null", std::move(textures), properties };
    }

    void ExtractAnimations(const unsigned int count, aiAnimation** const& in)
    {
        Animation animation;

        for (auto i = 0u; i < count; ++i)
            ProcessAnimation(in[i], animation);
    }

    void ProcessAnimation(aiAnimation* const& in, Animation& animation)
    {
        animation.name = in->mName.C_Str();

        animation.duration = in->mDuration;
        animation.ticksPerSecond = in->mTicksPerSecond;

        ExtractChannels(in->mNumChannels, in->mChannels, animation.channels);
    }

    void ExtractBones(const unsigned int count, aiBone** const& in, std::vector<Bone>& bones)
    {
        Bone bone;

        ProcessBone(in[0], bone);
    }

    void ProcessBone(aiBone* const& in, Bone& bone)
    {
        const auto& root = in->mNode;

        if (root == nullptr)
        {
            VK_ERROR("Root bone node is nullptr");
            return;
        }

        std::stack<aiNode*> stack;
        stack.emplace(root);
    }

    void ExtractChannels(const unsigned int count, aiNodeAnim** const& in, std::vector<Channel>& channels)
    {
        Channel channel;

        for (auto i = 0u; i < count; ++i)
        {
            ProcessChannel(in[i], channel);
            channels.emplace_back(channel);
        }
    }

    void ProcessChannel(aiNodeAnim* const& in, Channel& channel)
    {
        channel.name = in->mNodeName.C_Str();

        channel.positions.reserve(in->mNumPositionKeys);
        Channel::Position position {};

        for (auto i = 0u; i < in->mNumPositionKeys; ++i)
        {
            Vec3(in->mPositionKeys[i].mValue, position.position);
            position.timestamp = in->mPositionKeys[i].mTime;
        }

        channel.rotations.reserve(in->mNumRotationKeys);
        Channel::Rotation rotation {};

        for (auto i = 0u; i < in->mNumRotationKeys; ++i)
        {
            Quaternion(in->mRotationKeys[i].mValue, rotation.rotation);
            rotation.timestamp = in->mRotationKeys[i].mTime;
        }

        channel.scales.reserve(in->mNumScalingKeys);
        Channel::Scale scale {};

        for (auto i = 0u; i < in->mNumScalingKeys; ++i)
        {
            Vec3(in->mScalingKeys[i].mValue, scale.scale);
            scale.timestamp = in->mScalingKeys[i].mTime;
        }
    }

    void Mat4(const aiMatrix4x4& in, Math::Mat4& out)
    {
        out = Math::Mat4(in.a1, in.b1, in.c1, in.d1,
                         in.a2, in.b2, in.c2, in.d2,
                         in.a3, in.b3, in.c3, in.d3,
                         in.a4, in.b4, in.c4, in.d4);    
    }

    void Quaternion(const aiQuaternion& in, Math::Quaternion& out) { out = Math::Quaternion(in.w, in.x, in.y, in.z); }
    void Vec3(const aiVector3D& in, Math::Vec3& out) { out = Math::Vec3(in.x, in.y, in.z); }
    void Vec3(const aiColor3D& in, Math::Vec3& out)  { out = Math::Vec3(in.r, in.g, in.b); }
    void Vec2(const aiVector3D& in, Math::Vec2& out) { out = Math::Vec2(in.x, in.y); }

    Math::Vec3 ToGLM(const aiColor3D& v)
    {
        return {v.r, v.g, v.b};
    }

    Math::Vec3 ToGLM(const aiVector3D& v)
    {
        return {v.x, v.y, v.z};
    }
}