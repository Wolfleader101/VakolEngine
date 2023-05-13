#include "ModelLoader.hpp"

#include <Controller/AssetLoader/FileLoader.hpp>
#include <Controller/AssetLoader/TextureLoader.hpp>

#include <Model/Assets/md2.hpp>

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
        aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_GlobalScale;

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
    auto extract_bones(const aiMesh& mesh, std::vector<Vertex>& vertices)->void;
    auto extract_textures(const aiMaterial& material, aiTextureType type)->std::vector<Texture>;

    auto process_meshes(const aiScene& scene)->std::vector<Mesh>;
    auto process_mesh(const aiScene& scene, const aiMesh& assimp_mesh)->Mesh;
    auto process_material(const aiMaterial& material)->MaterialSpec;

    bool IS_CORE_ASSET = false; // A poor hack at best

    ::Model LoadModel(const std::string& path, const float scale, bool animated)
    {
        auto importer = Assimp::Importer{};

        IS_CORE_ASSET = path.find("coreAssets/");

        static_cast<void>(importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, scale));
        VK_ASSERT(FileExists(path), "File could not be found!");

        const auto* scene = importer.ReadFile(path.c_str(), ASSIMP_LOADER_OPTIONS);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            VK_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());

            importer.ReadFile("coreAssets/models/error.obj", aiProcess_Triangulate);

            animated = false; // force animations off
        }


        VK_TRACE("Number of Meshes: {0}", scene->mNumMeshes);
        if (animated)
            VK_TRACE("ANIMATED");
    // else
        return { process_meshes(*scene) };
    }

    // iteratively iterate through each node for meshes
    auto process_meshes(const aiScene& scene)->std::vector<Mesh>
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
                VK_TRACE(i);
                const auto& mesh = *scene.mMeshes[node.mMeshes[i]];
                
                meshes.push_back(process_mesh(scene, mesh));
            }

            for (unsigned int i = 0; i < node.mNumChildren; ++i)
                nodes.push(node.mChildren[i]);
        }

        return meshes;
    }

    auto process_mesh(const aiScene& scene, const aiMesh& assimp_mesh)->Mesh
    {
    	auto vertices = extract_vertices(assimp_mesh);
    	auto indices = extract_indices(assimp_mesh);
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

}