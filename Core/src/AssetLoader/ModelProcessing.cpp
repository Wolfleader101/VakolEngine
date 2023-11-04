#include "AssetLoader/ModelProcessing.hpp"

constexpr int ASSIMP_LOADER_OPTIONS =
    aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_GenBoundingBoxes |
    aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_SplitLargeMeshes |
    aiProcess_ValidateDataStructure | aiProcess_FindInvalidData | aiProcess_GlobalScale |
    aiProcess_PopulateArmatureData | aiProcess_FlipUVs | aiProcess_RemoveRedundantMaterials;

namespace Vakol
{
    ModelProcessing::ModelProcessing()
    {
        ERROR_MODEL_PATH = "coreAssets/models/error.obj";
    }

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
            return Rendering::Assets::VK_TEXTURE_DIFFUSE;
        case aiTextureType_SPECULAR:
            return Rendering::Assets::VK_TEXTURE_SPECULAR;
        case aiTextureType_AMBIENT:
            return Rendering::Assets::VK_TEXTURE_AMBIENT;
        case aiTextureType_EMISSIVE:
            return Rendering::Assets::VK_TEXTURE_EMISSION;
        case aiTextureType_HEIGHT:
            return Rendering::Assets::VK_TEXTURE_HEIGHT;
        case aiTextureType_NORMALS:
            return Rendering::Assets::VK_TEXTURE_NORMAL;
        case aiTextureType_UNKNOWN:
            return Rendering::Assets::VK_TEXTURE_NONE;
        default:
            break;
        }

        return Rendering::Assets::VK_TEXTURE_NONE;
    }

    Rendering::Assets::Model& ModelProcessing::FindModel(const GUID& ID)
    {
        if (!ModelExists(ID))
        {
            VK_ERROR("Unable to find unique model at ID: {0}", ID.ToString());
            return GetErrorModel(1.0f);
        }

        return m_models.at(ID);
    }

    Rendering::Assets::Model& ModelProcessing::GetModel(const GUID& ID, const std::string& path, const float scale)
    {
        if (!ModelExists(ID))
        {
            Rendering::Assets::Model& model =
                GetModel(path, scale); // load the model and put it into the loaded models map

            for (Rendering::Assets::Mesh& mesh : model.meshes)
                mesh.material.ID = Rendering::GenerateID();

            m_models[ID] = model;
        }

        return m_models.at(ID);
    }

    Rendering::Assets::Model& ModelProcessing::GetModel(const std::string& path, const float scale)
    {
        if (!UniqueModelExists(path))
        {
            if (!ImportModel(m_loadedModels[path], path.c_str(), scale))
            {
                VK_ERROR("Unable to get model at path {0}", path);
                return GetErrorModel(scale);
            }
        }

        return m_loadedModels.at(path);
    }

    Rendering::Assets::Model& ModelProcessing::GetErrorModel(const float scale)
    {
        if (!UniqueModelExists(ERROR_MODEL_PATH))
        {
            if (!ImportModel(m_loadedModels[ERROR_MODEL_PATH], ERROR_MODEL_PATH.c_str(), scale))
            {
                VK_CRITICAL("ERROR MODEL NOT FOUND!");
            }
        }

        return m_loadedModels.at(ERROR_MODEL_PATH);
    }

    bool ModelProcessing::UniqueModelExists(const std::string& path) const
    {
        return m_loadedModels.find(path) != m_loadedModels.end();
    }

    bool ModelProcessing::ModelExists(const GUID& ID) const
    {
        return m_models.find(ID) != m_models.end();
    }

    bool ModelProcessing::IsEmpty() const
    {
        return m_models.empty();
    }

    bool ModelProcessing::ImportModel(Rendering::Assets::Model& model, const char* path, const float scale)
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

        model.path = path;

        ExtractMeshes(*scene, model.meshes);

        return true;
    }

    void ModelProcessing::ExtractMeshes(const aiScene& scene, std::vector<Rendering::Assets::Mesh>& meshes)
    {
        // Fetch meshes in current node
        for (unsigned int i = 0; i < scene.mNumMeshes; ++i)
        {
            const auto& mesh = *scene.mMeshes[i];

            meshes.emplace_back(ProcessMesh(scene, mesh));
        }
    }

    Rendering::Assets::Mesh ModelProcessing::ProcessMesh(const aiScene& scene, const aiMesh& mesh)
    {
        std::vector<Rendering::Vertex> vertices;
        std::vector<unsigned int> indices;

        ExtractVertices(mesh, vertices, indices);

        const auto& material = ProcessMaterial(scene, scene.mMaterials[mesh.mMaterialIndex]);

        Rendering::Assets::Mesh pMesh = {Rendering::GenerateID(),
                                         mesh.mName.C_Str(),
                                         std::move(vertices),
                                         std::move(indices),
                                         std::vector<Rendering::Assets::Bone>(),
                                         material,
                                         {ToVec3(mesh.mAABB.mMin), ToVec3(mesh.mAABB.mMax), Math::Vec3(0.0f)}};

        pMesh.bounds.halfExtents = (pMesh.bounds.max - pMesh.bounds.min) / 2.0f;

        return pMesh;
    }

    void ModelProcessing::ExtractVertices(const aiMesh& mesh, std::vector<Rendering::Vertex>& vertices,
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

    Rendering::Assets::Material ModelProcessing::ProcessMaterial(const aiScene& scene, const aiMaterial* material)
    {
        std::vector<Rendering::Assets::Texture> textures;

        Rendering::Assets::MaterialProperties properties;

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

    std::vector<Rendering::Assets::Texture> ModelProcessing::ExtractTextures(const aiScene& scene,
                                                                             const aiMaterial* material,
                                                                             const aiTextureType type)
    {
        std::vector<Rendering::Assets::Texture> textures;

        const auto count = material->GetTextureCount(type);

        textures.reserve(count);

        for (unsigned int i = 0; i < count; ++i)
        {
            auto imported_path = aiString{};

            if (material->GetTexture(type, i, &imported_path) == AI_SUCCESS)
            {
                const std::string path = imported_path.C_Str();

                auto&& texture = Rendering::Assets::Texture{};

                if (const auto& embedded_texture = scene.GetEmbeddedTexture(path.c_str()))
                {
                    const auto size = static_cast<int>(embedded_texture->mWidth);

                    texture = AssetManager::GetTexture(embedded_texture->mFilename.C_Str(), GetTextureType(type), size,
                                                       embedded_texture->pcData);
                    textures.emplace_back(texture);
                }
                else
                {
                    texture = AssetManager::GetTexture(imported_path.C_Str(), GetTextureType(type));

                    textures.emplace_back(texture);
                }
            }
        }

        return textures;
    }
} // namespace Vakol
