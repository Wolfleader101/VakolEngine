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
         * \param levels mipmap levels
         * \return
         */
        Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int levels);
        /**
         * \brief
         * \param path the path of the texture
         * \param type the type of texture
         * \param size the size of the image buffer
         * \param data an address of image data embedded in memory
         * \param levels texture mipmap levels
         * \return
         */
        Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int size, const void* data,
                                               int levels);

        /**
         * \brief get the texture including it's pixel data.
         * \param path
         * \param type
         * \param width
         * \param height
         * \param channels
         * \param pixels
         */
        void GetTexture(const std::string& path, unsigned int type, int& width, int& height, int& channels,
                        unsigned char*& pixels);

        /**
         * \brief
         * \param paths
         * \return
         */
        std::vector<Rendering::Assets::Texture> GetTextures(std::vector<std::string>&& paths);

      private:
        /**
         * \brief
         * \param type
         * \return
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