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

    Rendering::Assets::Model& AssetLoader::FindModel(const std::string& path)
    {
        return m_modelLibrary.FindModel(path);
    }

    Rendering::Assets::Model& AssetLoader::GetModel(const std::string& path, const float scale)
    {
        return m_modelLibrary.GetModel(path, scale);
    }

    const std::vector<Rendering::Assets::Mesh>& AssetLoader::GetMeshes(const std::string& modelID)
    {
        return m_modelLibrary.FindModel(modelID).meshes;
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type,
                                                        const int levels)
    {
        return m_textureLibrary.GetTexture(path, type, levels);
    }

    void AssetLoader::GetTexture(const std::string& path, const unsigned int type, int& width, int& height, int& channels, unsigned char*& pixels)
    {
        return m_textureLibrary.GetTexture(path, type, width, height, channels, pixels);
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type,
                                                        const int size, const void* data, const int levels)
    {
        return m_textureLibrary.GetTexture(path, type, size, data, levels);
    }

    void AssetLoader::ReplaceTexture(const std::string& modelID, const std::string& srcPath, const unsigned int srcType,
                                     const std::string& dstPath, const unsigned int dstType)
    {
        const auto& model = FindModel(modelID);

        for (const auto& mesh : model.meshes)
        {
            auto& textures = mesh.material->textures;

            std::replace_if(
                textures.begin(), textures.end(),
                [&](const Rendering::Assets::Texture& texture) {
                    return texture.path == srcPath && texture.type == srcType;
                },
                GetTexture(dstPath, dstType));
        }
    }

} // namespace Vakol