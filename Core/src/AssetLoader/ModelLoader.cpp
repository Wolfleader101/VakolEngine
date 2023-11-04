#include "AssetLoader/ModelLoader.hpp"

using namespace Vakol::Rendering::Assets;

constexpr int ASSIMP_LOADER_OPTIONS =
    aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_GenBoundingBoxes |
    aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_SplitLargeMeshes |
    aiProcess_ValidateDataStructure | aiProcess_FindInvalidData | aiProcess_GlobalScale |
    aiProcess_PopulateArmatureData | aiProcess_FlipUVs | aiProcess_RemoveRedundantMaterials;

namespace Vakol
{
    /*Helper Functions*/
    static Math::Mat4 ToMat4(const aiMatrix4x4& m)
    {
        Math::Vec4 c1{m.a1, m.b1, m.c1, m.d1};
        Math::Vec4 c2{m.a2, m.b2, m.c2, m.d2};
        Math::Vec4 c3{m.a3, m.b3, m.c3, m.d3};
        Math::Vec4 c4{m.a4, m.b4, m.c4, m.d4};

        return {c1, c2, c3, c4};
    }

    static Math::Quat ToQuat(aiQuaternion& v)
    {
        return {v.w, v.x, v.y, v.z};
    }

    static Math::Vec4 ToVec4(aiColor3D& v)
    {
        return {v.r, v.g, v.b, 1.0f};
    }

    static Math::Vec3 ToVec3(const aiVector3D& v)
    {
        return {v.x, v.y, v.z};
    }

    static unsigned int GetTextureType(const aiTextureType type)
    {
        switch (type)
        {
        case aiTextureType_DIFFUSE:
            return VK_TEXTURE_DIFFUSE;
        case aiTextureType_SPECULAR:
            return VK_TEXTURE_SPECULAR;
        case aiTextureType_AMBIENT:
            return VK_TEXTURE_AMBIENT;
        case aiTextureType_EMISSIVE:
            return VK_TEXTURE_EMISSION;
        case aiTextureType_HEIGHT:
            return VK_TEXTURE_HEIGHT;
        case aiTextureType_NORMALS:
            return VK_TEXTURE_NORMAL;
        case aiTextureType_UNKNOWN:
            return VK_TEXTURE_NONE;
        default:
            break;
        }

        return VK_TEXTURE_NONE;
    }

    static void ExtractMeshes(const aiScene& scene, std::vector<Mesh>& meshes);
    static Mesh ProcessMesh(const aiScene& scene, const aiMesh& mesh);

    /*Mesh Internals*/
    static void ExtractVertices(const aiMesh& mesh, std::vector<Rendering::Vertex>& vertices,
                                std::vector<unsigned int>& indices);

    // static void ExtractBones(unsigned int count, aiBone** const& in, std::vector<Bone>& bones);
    // static void ProcessBone(aiBone* const& in, Bone& bone);

    /*Material*/
    static Material ProcessMaterial(const aiScene& scene, const aiMaterial* material);

    /*Material Internals*/
    static std::vector<Texture> ExtractTextures(const aiScene& scene, const aiMaterial* material, aiTextureType type);

    /*Animations*/
    // static void ExtractAnimations(unsigned int count, aiAnimation** const& in);
    // static void ProcessAnimation(aiAnimation* const& in, Animation& animation);

    // static void ExtractChannels(unsigned int count, aiNodeAnim** const& in, std::vector<Channel>& channels);
    // static void ProcessChannel(aiNodeAnim* const& in, Channel& channel);

    bool ImportModel(Model& model, const char* path, const float scale)
    {
        auto importer = Assimp::Importer{};

        if (!FileExists(path))
            VK_WARN("Model at path {0} was not found!", path);

        static_cast<void>(importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, scale));
        auto* scene = importer.ReadFile(path, ASSIMP_LOADER_OPTIONS);

        // if no scene flags were set (loader options) or the imported scene or root node is nullptr.
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            VK_ERROR("ASSIMP ERROR: {0}", importer.GetErrorString());

            return false;
        }

        // VK_TRACE("Stats for Model: {0}", path);

        model.path = path;

        ExtractMeshes(*scene, model.meshes);

        return true;
    }

    void ExtractMeshes(const aiScene& scene, std::vector<Mesh>& meshes)
    {
        // Fetch meshes in current node
        for (unsigned int i = 0; i < scene.mNumMeshes; ++i)
        {
            const auto& mesh = *scene.mMeshes[i];

            meshes.emplace_back(ProcessMesh(scene, mesh));
        }
    }

    Mesh ProcessMesh(const aiScene& scene, const aiMesh& mesh)
    {
        std::vector<Rendering::Vertex> vertices;
        std::vector<unsigned int> indices;

        ExtractVertices(mesh, vertices, indices);

        const auto& material = ProcessMaterial(scene, scene.mMaterials[mesh.mMaterialIndex]);

        Mesh pMesh = {Rendering::GenerateID(),
                      mesh.mName.C_Str(),
                      std::move(vertices),
                      std::move(indices),
                      std::vector<Bone>(),
                      material,
                      {ToVec3(mesh.mAABB.mMin), ToVec3(mesh.mAABB.mMax), Math::Vec3(0.0f)}};

        pMesh.bounds.halfExtents = (pMesh.bounds.max - pMesh.bounds.min) / 2.0f;

        return pMesh;
    }

    void ExtractVertices(const aiMesh& mesh, std::vector<Rendering::Vertex>& vertices,
                         std::vector<unsigned int>& indices)
    {
        vertices.reserve(mesh.mNumVertices);

        for (unsigned int i = 0; i < mesh.mNumVertices; ++i)
        {
            Rendering::Vertex vertex{};

            vertex.position = ToVec3(mesh.mVertices[i]);
            vertex.normal = ToVec3(mesh.mNormals[i]);

            if (mesh.HasTextureCoords(0))
            {
                vertex.uv = ToVec3(mesh.mTextureCoords[0][i]);

                if (mesh.HasTangentsAndBitangents())
                {
                    vertex.tangent = ToVec3(mesh.mTangents[i]);
                    vertex.bitangent = ToVec3(mesh.mBitangents[i]);
                }
            }

            std::fill(std::begin(vertex.boneIDs), std::end(vertex.boneIDs), -1);
            std::fill(std::begin(vertex.boneWeights), std::end(vertex.boneWeights), 0.0f);

            vertices.emplace_back(vertex);
        }

        for (auto i = 0u; i < mesh.mNumFaces; ++i)
            indices.insert(indices.end(), mesh.mFaces[i].mIndices,
                           mesh.mFaces[i].mIndices + mesh.mFaces[i].mNumIndices);
    }

    Material ProcessMaterial(const aiScene& scene, const aiMaterial* material)
    {
        std::vector<Texture> textures;

        MaterialProperties properties;

        aiColor3D ambient, diffuse, specular, emissive;

        material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
        material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
        material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
        material->Get(AI_MATKEY_SHININESS, properties.shininess);
        material->Get(AI_MATKEY_OPACITY, properties.opacity);

        properties.ambient_color = ToVec4(ambient);
        properties.diffuse_color = ToVec4(diffuse);
        properties.specular_color = ToVec4(specular);
        properties.emissive_color = ToVec4(emissive);

        if (properties.shininess < 1.0f)
            properties.shininess = 32.0f;

        auto&& diffuse_maps = ExtractTextures(scene, material, aiTextureType_DIFFUSE);
        auto&& specular_maps = ExtractTextures(scene, material, aiTextureType_SPECULAR);
        auto&& ambient_maps = ExtractTextures(scene, material, aiTextureType_AMBIENT);
        auto&& height_maps = ExtractTextures(scene, material, aiTextureType_HEIGHT);
        auto&& emission_maps = ExtractTextures(scene, material, aiTextureType_EMISSIVE);
        auto&& normal_maps = ExtractTextures(scene, material, aiTextureType_NORMALS);

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

        return {material->GetName().C_Str(), "null", "null", std::move(textures), properties};
    }

    std::vector<Texture> ExtractTextures(const aiScene& scene, const aiMaterial* material, const aiTextureType type)
    {
        std::vector<Texture> textures;

        const auto count = material->GetTextureCount(type);

        textures.reserve(count);

        for (unsigned int i = 0; i < count; ++i)
        {
            auto imported_path = aiString{};

            if (material->GetTexture(type, i, &imported_path) == AI_SUCCESS)
            {
                const std::string path = imported_path.C_Str();

                auto&& texture = Texture{};

                if (const auto& embedded_texture = scene.GetEmbeddedTexture(path.c_str()))
                {
                    const auto size = static_cast<int>(embedded_texture->mWidth);

                    texture = AssetLoader::GetTexture(embedded_texture->mFilename.C_Str(), GetTextureType(type), size,
                                                      embedded_texture->pcData);
                    textures.emplace_back(texture);
                }
                else
                {
                    texture = AssetLoader::GetTexture(imported_path.C_Str(), GetTextureType(type));

                    textures.emplace_back(texture);
                }
            }
        }

        return textures;
    }

    // void ExtractAnimations(const unsigned int count, aiAnimation** const& in)
    //{
    //     Animation animation;

    //    //for (auto i = 0u; i < count; ++i)
    //    //    ProcessAnimation(in[i], animation);
    //}

    // void ProcessAnimation(aiAnimation* const& in, Animation& animation)
    //{
    //     animation.name = in->mName.C_Str();

    //    animation.duration = in->mDuration;
    //    animation.ticksPerSecond = in->mTicksPerSecond;

    //    //ExtractChannels(in->mNumChannels, in->mChannels, animation.channels);
    //}

    // void ExtractBones(const unsigned int count, aiBone** const& in, std::vector<Bone>& bones)
    //{
    //     Bone bone;

    //    ProcessBone(in[0], bone);
    //}

    // void ProcessBone(aiBone* const& in, Bone& bone)
    //{
    //     const auto& root = in->mNode;

    //    if (root == nullptr)
    //    {
    //        VK_ERROR("Root bone node is nullptr");
    //        return;
    //    }

    //    std::stack<aiNode*> stack;
    //    stack.emplace(root);
    //}

    // void ExtractChannels(const unsigned int count, aiNodeAnim** const& in, std::vector<Channel>& channels)
    //{
    //     Channel channel;

    //    for (auto i = 0u; i < count; ++i)
    //    {
    //        ProcessChannel(in[i], channel);
    //        channels.emplace_back(channel);
    //    }
    //}

    void ProcessChannel(aiNodeAnim* const& in, Channel& channel)
    {
        channel.name = in->mNodeName.C_Str();

        channel.positions.reserve(in->mNumPositionKeys);
        Channel::Position position{};

        for (auto i = 0u; i < in->mNumPositionKeys; ++i)
        {
            position.position = ToVec3(in->mPositionKeys[i].mValue);
            position.timestamp = in->mPositionKeys[i].mTime;

            channel.positions.emplace_back(position);
        }

        channel.rotations.reserve(in->mNumRotationKeys);
        Channel::Rotation rotation{};

        for (auto i = 0u; i < in->mNumRotationKeys; ++i)
        {
            rotation.rotation = ToQuat(in->mRotationKeys[i].mValue);
            rotation.timestamp = in->mRotationKeys[i].mTime;

            channel.rotations.emplace_back(rotation);
        }

        channel.scales.reserve(in->mNumScalingKeys);
        Channel::Scale scale{};

        for (auto i = 0u; i < in->mNumScalingKeys; ++i)
        {
            scale.scale = ToVec3(in->mScalingKeys[i].mValue);
            scale.timestamp = in->mScalingKeys[i].mTime;

            channel.scales.emplace_back(scale);
        }
    }
} // namespace Vakol