#include "AssetLoader/AssetManager.hpp"

namespace Vakol
{
    AssetManager::AssetManager()
    {
        // Setting the default file paths
        modelPath = "assets/models/";
        texturePath = "assets/textures/";
        shaderPath = "assets/shaders/";
    }

    std::string AssetManager::GetFolderPath(AssetManager::FileType outputType)
    {
        // Check the type of folder path that is being given and return it
        switch (outputType)
        {
        case MODELS:
            return modelPath;

            break;
        case TEXTURES:
            return texturePath;

            break;
        case SHADERS:
            return shaderPath;

            break;
        default:
            VK_ERROR("The Filepath type is incorrect!");

            return "";
        }
    }

    void AssetManager::SetFolderPath(AssetManager::FileType outputType, std::string newPath)
    {
        // Check the type of folder path that is being given and return it
        switch (outputType)
        {
        case MODELS:
            modelPath = newPath;

            break;
        case TEXTURES:
            texturePath = newPath;

            break;
        case SHADERS:
            shaderPath = newPath;

            break;
        default:
            VK_ERROR("The Filepath type is incorrect!");
        }
    }

    Rendering::Assets::Model& AssetManager::FindModel(const GUID& ID)
    {
        return m_modelProcessing.FindModel(ID);
    }

    Rendering::Assets::Model& AssetManager::GetModel(const GUID& ID, const std::string& path, const float scale)
    {
        return m_modelProcessing.GetModel(ID, path, scale);
    }

    const std::vector<Rendering::Assets::Mesh>& AssetManager::GetMeshes(const GUID& ID)
    {
        return m_modelProcessing.FindModel(ID).meshes;
    }

    Rendering::Assets::Texture& AssetManager::GetTexture(const std::string& path, const unsigned int type)
    {
        return m_textureProcessing.GetTexture(path, type);
    }

    Rendering::Assets::Texture& AssetManager::GetTexture(const std::string& path, const unsigned int type, int& width,
                                                         int& height, int& channels, unsigned char*& pixels)
    {
        return m_textureProcessing.GetTexture(path, type, width, height, channels, pixels);
    }

    Rendering::Assets::Texture& AssetManager::GetTexture(const std::string& path, const unsigned int type,
                                                         const int size, const void* data)
    {
        return m_textureProcessing.GetTexture(path, type, size, data);
    }

    void AssetManager::ReplaceTexture(const GUID& modelID, const std::string& srcPath, const unsigned int srcType,
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

    bool AssetManager::IsExistingModel(const GUID& modelID)
    {
        return m_modelProcessing.ModelExists(modelID);
    }

    AssetManager::~AssetManager()
    {
        VK_INFO("AssetManager cleanup started.");

        // Clear the vectors
        m_models.clear();
        m_textures.clear();
        m_shaders.clear();

        // Clear the maps
        m_modelIdentifier.clear();
        m_textureIdentifier.clear();
        m_shaderIdentifier.clear();

        VK_INFO("AssetManager cleanup finished.");
    }
} // namespace Vakol
