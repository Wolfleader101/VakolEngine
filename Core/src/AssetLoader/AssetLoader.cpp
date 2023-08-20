#include "AssetLoader/AssetLoader.hpp"

#include "AssetLoader/TextureLoader.hpp"
#include "Rendering/RenderAPI.hpp"

namespace Vakol
{
    std::string AssetLoader::model_path = "assets/models/";
    std::string AssetLoader::texture_path = "assets/textures/";
    std::string AssetLoader::shader_path = "assets/shaders/";

    ModelLibrary AssetLoader::m_modelLibrary;
    MaterialLibrary AssetLoader::m_materialLibrary;

    std::unordered_map<std::string, Rendering::Assets::Texture> AssetLoader::m_textures;

    Rendering::Assets::Model& AssetLoader::GetModel(const std::string& path, const float scale)
    {
        return m_modelLibrary.GetModel(path, scale);
    }

    Rendering::Assets::Material& AssetLoader::GetMaterial(const std::string& materialID)
    {
        return m_materialLibrary.GetMaterial(materialID);
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type)
    {
        if (m_textures.find(path) == m_textures.end())
        {
            Rendering::Assets::Texture texture;

            texture.path = path;
            texture.type = type;

            unsigned char* pixels = nullptr;

            ImportTexture(path, texture.width, texture.height, texture.channels, pixels);
            texture.ID = Rendering::RenderAPI::GenerateTexture(texture.width, texture.height, texture.channels, pixels);

            m_textures[path] = std::move(texture);

            return m_textures.at(path);
        }

        return m_textures.at(path);
    }

    Rendering::Assets::Texture& AssetLoader::GetTexture(const std::string& path, const unsigned int type,
                                                        const int size, const void* buffer)
    {
        if (m_textures.find(path) == m_textures.end())
        {
            Rendering::Assets::Texture texture;

            texture.path = path;
            texture.type = type;
            texture.embedded = true;

            unsigned char* pixels = nullptr;

            ImportTexture(buffer, size, texture.width, texture.height, texture.channels, pixels);
            texture.ID = Rendering::RenderAPI::GenerateTexture(texture.width, texture.height, texture.channels, pixels);

            m_textures[path] = std::move(texture);

            return m_textures.at(path);
        }

        return m_textures.at(path);
    }

    void AssetLoader::AddTexture(const std::string& materialID, const Rendering::Assets::Texture& texture)
    {
        m_materialLibrary.AddTexture(materialID, texture);
    }

    void AssetLoader::ReplaceTexture(const std::string& materialID, const std::string& src, const std::string& dst,
                                     const std::string& type)
    {
        m_materialLibrary.ReplaceTexture(materialID, src, dst, Rendering::Assets::ToTextureType(type));
    }

    void AssetLoader::AddMaterial(const Rendering::Assets::Material& material)
    {
        m_materialLibrary.AddMaterial(material);

        MaterialLibrary::SetupMaterial(material);
    }

    bool AssetLoader::GetTextures(const std::string& materialID, std::vector<Rendering::Assets::Texture>& textures)
    {
        return m_materialLibrary.GetTextures(materialID, textures);
    }

} // namespace Vakol
