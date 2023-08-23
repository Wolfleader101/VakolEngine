#include "AssetLoader/TextureLibrary.hpp"

#include "AssetLoader/FileLoader.hpp"
#include "AssetLoader/TextureLoader.hpp"
#include "Logger/Logger.hpp"

#include "Rendering/RenderAPI.hpp"

#include "Rendering/Assets/Texture.hpp"

namespace Vakol
{
    const std::string ERROR_TEXTURE_PATH = "coreAssets/textures/error.png";

    Rendering::Assets::Texture& TextureLibrary::GetTexture(const std::string& path, const unsigned int type,
                                                           const int levels)
    {
        if (!FileExists(path))
        {
            return GetErrorTexture(type);
        }

        if (!FindTexture(path, type))
        {
            Rendering::Assets::Texture texture;

            texture.path = path;
            texture.type = type;

            unsigned char* pixels = nullptr;

            ImportTexture(path, texture.width, texture.height, texture.channels, pixels);
            texture.ID =
                Rendering::RenderAPI::GenerateTexture(levels, texture.width, texture.height, texture.channels, pixels);

            m_textures[std::make_pair(path, type)] = std::move(texture);
        }

        return m_textures.at(std::make_pair(path, type));
    }

    Rendering::Assets::Texture& TextureLibrary::GetTexture(const std::string& path, const unsigned int type,
                                                           const int size, const void* data, const int levels)
    {

        if (!FindTexture(path, type))
        {
            Rendering::Assets::Texture texture;

            texture.path = path;
            texture.type = type;
            texture.embedded = true;

            unsigned char* pixels = nullptr;

            ImportTexture(data, size, texture.width, texture.height, texture.channels, pixels);
            texture.ID =
                Rendering::RenderAPI::GenerateTexture(levels, texture.width, texture.height, texture.channels, pixels);

            m_textures[std::make_pair(path, type)] = std::move(texture);
        }

        return m_textures.at(std::make_pair(path, type));
    }

    Rendering::Assets::Texture& TextureLibrary::GetErrorTexture(const unsigned int type)
    {
        const auto& error = GetTexture(ERROR_TEXTURE_PATH, type, 1);

        if (!FindTexture(ERROR_TEXTURE_PATH, type))
        {
            m_textures[std::make_pair(ERROR_TEXTURE_PATH, type)] = error;
        }

        return m_textures.at(std::make_pair(ERROR_TEXTURE_PATH, type));
    }

    bool TextureLibrary::FindTexture(const std::string& path, const unsigned int type) const
    {
        return m_textures.find(std::make_pair(path, type)) != m_textures.end();
    }
} // namespace Vakol