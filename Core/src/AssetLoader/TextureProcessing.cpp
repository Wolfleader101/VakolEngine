#include "AssetLoader/TextureProcessing.hpp"

namespace Vakol
{
    TextureProcessing::TextureProcessing()
	{
        ERROR_TEXTURE_PATH = "coreAssets/textures/error.png";
	}
    
    Rendering::Assets::Texture& TextureProcessing::GetTexture(const std::string& path, const unsigned int type)
    {
        if (!FileExists(path))
        {
            VK_ERROR("Texture at path: {0} could not be found!", path);

            return GetErrorTexture(type);
        }

        if (!FindTexture(path, type))
        {
            Rendering::Assets::Texture texture;

            texture.path = path;
            texture.type = type;
            texture.embedded = false;

            unsigned char* pixels = nullptr;

            ImportTexture(path, texture.width, texture.height, texture.channels, pixels);

            int levelX = texture.width;
            int levelY = texture.height;

            while (levelX != 1 && levelY != 1)
            {
                levelX = static_cast<int>(floor(levelX / 2));
                levelY = static_cast<int>(floor(levelY / 2));

                texture.levels++;
            }

            texture.ID = Rendering::RenderAPI::GenerateTexture(texture.levels, texture.width, texture.height,
                                                               texture.channels, pixels);

            m_textures[std::make_pair(path, type)] = std::move(texture);
        }

        return m_textures.at(std::make_pair(path, type));
    }

    Rendering::Assets::Texture& TextureProcessing::GetTexture(const std::string& path, const unsigned int type,
                                                             const int size, const void* data)
    {
        if (!FindTexture(path, type))
        {
            Rendering::Assets::Texture texture;

            texture.path = path;
            texture.type = type;
            texture.embedded = true;

            unsigned char* pixels = nullptr;

            if (!data || size <= 0)
            {
                VK_ERROR("Invalid Texture Data!");
                return GetErrorTexture(type);
            }

            ImportTexture(data, size, texture.width, texture.height, texture.channels, pixels);

            int levelX = texture.width;
            int levelY = texture.height;

            while (levelX != 1 && levelY != 1)
            {
                levelX = static_cast<int>(floor(levelX / 2));
                levelY = static_cast<int>(floor(levelY / 2));

                texture.levels++;
            }

            if (!pixels)
            {
                VK_ERROR("Unable to import texture at path: {0}", path);

                return GetErrorTexture(type);
            }

            texture.ID = Rendering::RenderAPI::GenerateTexture(texture.levels, texture.width, texture.height,
                                                               texture.channels, pixels);

            m_textures[std::make_pair(path, type)] = std::move(texture);
        }

        return m_textures.at(std::make_pair(path, type));
    }

    Rendering::Assets::Texture& TextureProcessing::GetTexture(const std::string& path, const unsigned int type,
                                                             int& width, int& height, int& channels,
                                                             unsigned char*& pixels)
    {
        if (!FindTexture(path, type))
        {
            Rendering::Assets::Texture texture;

            texture.path = path;
            texture.type = type;
            texture.embedded = false;

            ImportTexture(path, width, height, channels, pixels);

            texture.width = width;
            texture.height = height;
            texture.channels = channels;

            int levelX = texture.width;
            int levelY = texture.height;

            if (width > 0 && height > 0)
            {
                while (levelX != 1 && levelY != 1)
                {
                    levelX = static_cast<int>(floor(levelX / 2));
                    levelY = static_cast<int>(floor(levelY / 2));

                    texture.levels++;
                }
            }

            if (!pixels)
            {
                VK_ERROR("Unable to get texture pixels!");

                return GetErrorTexture(type);
            }

            m_textures[std::make_pair(path, type)] = std::move(texture);
        }

        return m_textures.at(std::make_pair(path, type));
    }

    std::vector<Rendering::Assets::Texture> TextureProcessing::GetTextures(std::vector<std::string>&& paths)
    {
        std::vector<Rendering::Assets::Texture> textures;

        for (const auto& path : paths)
        {
            const auto& texture = GetTexture(path, Rendering::Assets::VK_TEXTURE_DIFFUSE);
            textures.emplace_back(texture);
        }

        return textures;
    }

    Rendering::Assets::Texture& TextureProcessing::GetErrorTexture(const unsigned int type)
    {
        const auto& error = GetTexture(ERROR_TEXTURE_PATH, type);

        if (!FindTexture(ERROR_TEXTURE_PATH, type))
        {
            m_textures[std::make_pair(ERROR_TEXTURE_PATH, type)] = error;
        }

        return m_textures.at(std::make_pair(ERROR_TEXTURE_PATH, type));
    }

    bool TextureProcessing::FindTexture(const std::string& path, const unsigned int type) const
    {
        return m_textures.find(std::make_pair(path, type)) != m_textures.end();
    }

    bool TextureProcessing::IsEmpty() const
    {
        return m_textures.empty();
    }

    void TextureProcessing::ImportTexture(const std::string& path, int& width, int& height, int& channels,
                                         unsigned char*& pixels)
    {
        if (!FileExists(path))
        {
            VK_ERROR("Could not find texture file at path: {0}", path);

            return;
        }

        pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!pixels)
        {
            VK_ERROR("Unable to Import Texture at path: {0}", path);

            stbi_image_free(pixels);
            pixels = nullptr;
        }
    }

    void TextureProcessing::ImportTexture(const void* data, const int length, int& width, int& height, int& channels,
                                         unsigned char*& pixels)
    {
        pixels = stbi_load_from_memory(static_cast<const stbi_uc* const>(data), length, &width, &height, &channels, 0);

        if (!pixels)
        {
            VK_ERROR("Unable to import embedded texture");

            stbi_image_free(pixels);
            pixels = nullptr;
        }
    }

    void TextureProcessing::FreeTexture(void* pixels)
    {
        stbi_image_free(pixels);
        pixels = nullptr;
    }
} // namespace Vakol
