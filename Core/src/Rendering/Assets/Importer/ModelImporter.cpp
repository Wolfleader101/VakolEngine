#include "ModelImporter.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Controller/AssetLoader/FileLoader.hpp"
#include "Controller/Logger.hpp"

#include "../Material.hpp"
#include "../Animation.hpp"

#include "Rendering/RenderData.hpp"

#include "Rendering/MaterialLibrary.hpp"

#include "Math/Math.hpp"

using namespace Vakol::Rendering::Assets;

constexpr unsigned int ASSIMP_LOADER_OPTIONS =
    aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes | aiProcess_JoinIdenticalVertices |
    aiProcess_ImproveCacheLocality | aiProcess_SplitLargeMeshes | aiProcess_ValidateDataStructure |
    aiProcess_FindInvalidData | aiProcess_GlobalScale;


namespace Vakol::Rendering::Assets::Importer
{
    /*Helper Functions*/
    static void Vec3(const aiVector3D& in, Math::Vec3& out);
    static void Vec3(const aiColor3D& in, Math::Vec3& out);
    static void Vec2(const aiVector3D& in, Math::Vec2& out);

    /*Model*/
    static void ProcessModel(const aiScene& scene, Model& model);

    /*Meshes*/
    static void ExtractMeshes(unsigned int count, aiMesh** const& in, std::vector<Mesh>& meshes);
    static void ProcessMesh(aiMesh* const& in, Mesh& mesh);

    /*Mesh Internals*/
    static void ExtractVertices(aiMesh* const& in, Mesh& mesh);
    //static void ExtractBones();

    /*Materials*/
    static void ExtractMaterials(unsigned int count, aiMaterial** const& in);
    static void ProcessMaterial(aiMaterial* const& in, Material& material);

    /*Material Textures*/
    static void ExtractMaterialTextures(aiTextureType type, aiMaterial* const& in);

    /*Embedded Textures*/
    static void ExtractEmbeddedTextures(unsigned int count, aiTexture** const& in);
    static void ProcessEmbeddedTexture(aiTexture* const& in, Rendering::Texture& texture);

    /*Animations*/
    static void ExtractAnimations(unsigned int count, aiAnimation** const& in);
    static void ProcessAnimation(aiAnimation* const& in, Animation& animation);

    static void ExtractChannels(unsigned int count, aiNodeAnim** const& in);

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

        ProcessModel(*scene, model);

        success = true;

        return model;
    }

    void ProcessModel(const aiScene& scene, Model& model)
    {
        model.name = scene.mName.C_Str();

        if (scene.HasMeshes())
            ExtractMeshes(scene.mNumMeshes, scene.mMeshes, model.meshes);
        else
            VK_WARN("Model has no meshes!");

        if (scene.HasMaterials())
            ExtractMaterials(scene.mNumMaterials, scene.mMaterials);
        else
            VK_WARN("Model has no materials!");

        if (scene.HasTextures()) 
            ExtractEmbeddedTextures(scene.mNumTextures, scene.mTextures);

        if (scene.HasAnimations()) 
            ExtractAnimations(scene.mNumAnimations, scene.mAnimations);
    }

    void ExtractMeshes(const unsigned int count, aiMesh** const& in, std::vector<Mesh>& meshes)
    {
        VK_TRACE("Mesh Count: {0}", count);

        Mesh mesh;

        for (auto i = 0u; i < count; ++i) 
        {
            ProcessMesh(in[i], mesh);

            meshes.emplace_back(mesh);
        }
    }

    void ExtractMaterials(const unsigned int count, aiMaterial** const& in)
    {
        VK_TRACE("Material Count: {0}", count);

        Material material;

        for (auto i = 0u; i < count; ++i)
        {
            ProcessMaterial(in[i], material);
            MaterialLibrary::AddMaterial(material);
        }
    }

    void ProcessMesh(aiMesh* const& in, Mesh& mesh)
    {
        mesh.name = in->mName.C_Str();

        Vec3(in->mAABB.mMin, mesh.bounds.min);
        Vec3(in->mAABB.mMax, mesh.bounds.max);

        if (!in->HasPositions()) {
            VK_ERROR("ASSIMP was unable to detect any vertices on the mesh.");
            return;
        }

        if (!in->HasFaces()) {
            VK_ERROR("ASSIMP was unable to detect any faces (triangles) on the mesh.");
            return;
        }

        ExtractVertices(in, mesh);
    }

    void ProcessMaterial(aiMaterial* const& in, Material& material)
    {
        material.name = in->GetName().C_Str();

        aiColor3D ambient_color, diffuse_color, specular_color, emission_color;

        in->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color);
        in->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color);
        in->Get(AI_MATKEY_COLOR_SPECULAR, specular_color);
        in->Get(AI_MATKEY_COLOR_EMISSIVE, emission_color);

        in->Get(AI_MATKEY_SHININESS, material.properties.shininess);
        in->Get(AI_MATKEY_EMISSIVE_INTENSITY, material.properties.intensity);
        in->Get(AI_MATKEY_ENABLE_WIREFRAME, material.properties.wireframe);

        Vec3(diffuse_color, material.properties.diffuse_color);
        Vec3(ambient_color, material.properties.ambient_color);
        Vec3(specular_color, material.properties.specular_color);
        Vec3(emission_color, material.properties.emissive_color);

        ExtractMaterialTextures(aiTextureType_DIFFUSE, in);
        ExtractMaterialTextures(aiTextureType_SPECULAR, in);
        ExtractMaterialTextures(aiTextureType_AMBIENT, in);
        ExtractMaterialTextures(aiTextureType_EMISSIVE, in);
        ExtractMaterialTextures(aiTextureType_HEIGHT, in);
        ExtractMaterialTextures(aiTextureType_NORMALS, in);
    }

    void ExtractMaterialTextures(const aiTextureType type, aiMaterial* const& in)
    {
        const auto count = in->GetTextureCount(type);

        if (count == 0)
            return;

        aiString str;

        for (auto i = 0u; i < count; ++i) {
            if (in->GetTexture(type, i, &str) == AI_SUCCESS) {
                // Add texture to Asset loader
            }
        }
    }

    void ExtractVertices(aiMesh* const& in, Mesh& mesh)
    {
        Rendering::Vertex vertex{};

        mesh.vertices.reserve(in->mNumVertices);

        for (auto i = 0u; i < in->mNumVertices; ++i) {
            Vec3(in->mVertices[i], vertex.position);

            if (in->HasNormals()) Vec3(in->mNormals[i], vertex.normal);

            if (in->HasTextureCoords(0)) {
                Vec2(in->mTextureCoords[0][i], vertex.uv);

                if (in->HasTangentsAndBitangents()) {
                    Vec3(in->mTangents[i], vertex.tangent);
                    Vec3(in->mBitangents[i], vertex.bitangent);
                }
            }

            mesh.vertices.emplace_back(vertex);
        }

        for (auto i = 0u; i < in->mNumFaces; ++i)
            mesh.indices.insert(mesh.indices.end(), in->mFaces[i].mIndices,in->mFaces[i].mIndices + in->mFaces[i].mNumIndices);
    }

    void ExtractEmbeddedTextures(const unsigned int count, aiTexture** const& in)
    {
        Texture texture;

        for (auto i = 0u; i < count; ++i) 
            ProcessEmbeddedTexture(in[i], texture);
    }

    void ProcessEmbeddedTexture(aiTexture* const& in, Texture& texture)
    {
        texture.path = in->mFilename.C_Str();

        texture.width = static_cast<int>(in->mWidth);
        texture.height = static_cast<int>(in->mHeight);
    }

    void ExtractAnimations(const unsigned int count, aiAnimation** const& in)
    {
        VK_TRACE("Animation Count: {0}", count);


        Animation animation;

        for (auto i = 0u; i < count; ++i) 
            ProcessAnimation(in[i], animation);
    }

    void ProcessAnimation(aiAnimation* const& in, Animation& animation)
    {
        //in->mNumChannels
    }

    void ExtractChannels(const unsigned int count, aiNodeAnim** const& in)
    {
        
    }

    void Vec3(const aiVector3D& in, Math::Vec3& out) { out = Math::Vec3(in.x, in.y, in.z); }
    void Vec3(const aiColor3D& in, Math::Vec3& out)  { out = Math::Vec3(in.r, in.g, in.b); }
    void Vec2(const aiVector3D& in, Math::Vec2& out) { out = Math::Vec2(in.x, in.y); }
}