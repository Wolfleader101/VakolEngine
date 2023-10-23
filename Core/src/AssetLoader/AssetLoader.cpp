#include "AssetLoader/AssetLoader.hpp"

#include "Logger/Logger.hpp"

#include <algorithm>

namespace Vakol
{
    std::string AssetLoader::model_path = "assets/models/";
    std::string AssetLoader::texture_path = "assets/textures/";
    std::string AssetLoader::shader_path = "assets/shaders/";

    ModelLibrary AssetLoader::m_modelLibrary;
    TextureLibrary AssetLoader::m_textureLibrary;

    Rendering::Assets::Model& AssetLoader::FindModel(const GUID& ID)
    {
        return m_modelLibrary.FindModel(ID);
    }

    Rendering::Assets::Model& AssetLoader::GetModel(const GUID& ID, const std::string& path, const float scale)
    {
        return m_modelLibrary.GetModel(ID, path, scale);
    }

    const std::vector<Rendering::Assets::Mesh>& AssetLoader::GetMeshes(const GUID& ID)
    {
        return m_modelLibrary.FindModel(ID).meshes;
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type)
    {
        return m_textureLibrary.GetTexture(path, type);
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type, int& width,
                                                        int& height, int& channels, unsigned char*& pixels)
    {
        return m_textureLibrary.GetTexture(path, type, width, height, channels, pixels);
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type,
                                                        const int size, const void* data)
    {
        return m_textureLibrary.GetTexture(path, type, size, data);
    }

    void AssetLoader::ReplaceTexture(const GUID& modelID, const std::string& srcPath, const unsigned int srcType,
                                     const std::string& dstPath, const unsigned int dstType)
    {
        Rendering::Assets::Model& model = FindModel(modelID);

        for (Rendering::Assets::Mesh& mesh : model.meshes)
        {
            auto& textures = mesh.material.textures;

            std::replace_if(
                textures.begin(), textures.end(),
                [&](const Rendering::Assets::Texture& texture) {
                    return texture.path == srcPath && texture.type == srcType;
                },
                GetTexture(dstPath, dstType));
        }
    }

    bool AssetLoader::IsExistingModel(const GUID& modelID)
    {
        return m_modelLibrary.ModelExists(modelID);
    }

} // namespace Vakol