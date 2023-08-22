#include "AssetLoader/TextureLibrary.hpp"

#include "AssetLoader/TextureLoader.hpp"
#include "Logger/Logger.hpp"

#include "Rendering/RenderAPI.hpp"

#include "Rendering/Assets/Texture.hpp"

namespace Vakol
{
    Rendering::Assets::Texture& TextureLibrary::GetTexture(const std::string& path, const unsigned int type)
    {
        if (!FindTexture(path, type))
        {
            Rendering::Assets::Texture texture;

            texture.path = path;
            texture.type = type;

            unsigned char* pixels = nullptr;

            ImportTexture(path, texture.width, texture.height, texture.channels, pixels);
            texture.ID = Rendering::RenderAPI::GenerateTexture(texture.width, texture.height, texture.channels, pixels);

            m_textures[std::make_pair(path, type)] = std::move(texture);
        }

        return m_textures.at(std::make_pair(path, type));
    }

    Rendering::Assets::Texture& TextureLibrary::GetTexture(const std::string& path, const unsigned int type, const int size, const void* data)
    {
        if (!FindTexture(path, type))
        {
            Rendering::Assets::Texture texture;

            texture.path = path;
            texture.type = type;
            texture.embedded = true;

            unsigned char* pixels = nullptr;

            ImportTexture(data, size, texture.width, texture.height, texture.channels, pixels);
            texture.ID = Rendering::RenderAPI::GenerateTexture(texture.width, texture.height, texture.channels, pixels);

            m_textures[std::make_pair(path, type)] = std::move(texture);
        }

        return m_textures.at(std::make_pair(path, type));
    }

    bool TextureLibrary::FindTexture(const std::string& path, const unsigned int type) const
    {
        return m_textures.find(std::make_pair(path, type)) != m_textures.end();
    }
}