#pragma once

#include <string>
#include <vector>

#include <unordered_map>

/**
 * \brief In order to use an unordered map with a std::pair as a key, you need to create a custom hashing function
 */
struct PairHash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const
    {
        const auto& v1 = std::hash<T1>{}(pair.first);
        const auto& v2 = std::hash<T2>{}(pair.second);

        return v1 ^ v2;
    }
};

namespace Vakol
{
    const std::string ERROR_TEXTURE_PATH = "coreAssets/textures/error.png";

    namespace Rendering::Assets
    {
        struct Texture;
    }

    class TextureLibrary
    {
      public:
        /**
         * \brief
         * \param path
         * \param type
         * \return
         */
        Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type);
        /**
         * \brief get a texture froma  path
         * \param path the path of the texture
         * \param type the type of texture
         * \param size the size of the image buffer
         * \param data an address of image data embedded in memory
         * \return the texture
         */
        Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int size, const void* data);

        /**
         * \brief get the texture including it's pixel data.
         * \param path the path of the image texture
         * \param type the type of image texture
         * \param width the width of the image texture
         * \param height the height of the image texture
         * \param channels the number of color channels of the image texture
         * \param pixels the pixels of the texture
         */
        Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int& width, int& height,
                                               int& channels, unsigned char*& pixels);

        /**
         * \brief get the textures from a series of paths from a cubemap
         * \param paths the paths of a cubemap's faces
         * \return a vector of textures for each face of a cubemap
         */
        std::vector<Rendering::Assets::Texture> GetTextures(std::vector<std::string>&& paths);

        /**
         * \brief checks if a texture library is empty or not.
         * \return whether or not the texture library is empty
         */
        bool IsEmpty() const;

      private:
        /**
         * \brief Get the error texture
         * \param type the type of texture
         * \return the error texture
         */
        Rendering::Assets::Texture& GetErrorTexture(unsigned int type);
        /**
         * \brief
         */
        std::unordered_map<std::pair<std::string, unsigned int>, Rendering::Assets::Texture, PairHash> m_textures;
        /**
         * \brief
         * \param path
         * \param type
         * \return
         */
        [[nodiscard]] bool FindTexture(const std::string& path, unsigned int type) const;
    };
} // namespace Vakol