#pragma once

// This is a preprocessor directive that tells the compiler to include the implementation of the stb_image library
#define STB_IMAGE_IMPLEMENTATION

// C++ Standard Libraries
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Imported Libraries
#include <stb_image.h>

// Vakol Libraries
#include "AssetLoader/FileLoader.hpp"

#include "Rendering/Assets/Texture.hpp"
#include "Rendering/RenderAPI.hpp"

#include "Logger/Logger.hpp"

namespace Vakol
{
    namespace Rendering::Assets
    {
        struct Texture;
    }

    class TextureProcessing
    {
      public:
        /**
         * @brief Gets the texture from a path and type
         * @param path The path of the texture
         * @param type The type of texture
         * @return The texture
         */
        Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type);
        /**
         * @brief Get a texture from a path, type and size
         * @param path The path of the texture
         * @param type The type of texture
         * @param size The size of the image buffer
         * @param data An address of image data embedded in memory
         * @return The texture
         */
        Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int size, const void* data);

        /**
         * @brief Get the texture including it's pixel data
         * @param path The path of the image texture
         * @param type The type of image texture
         * @param width The width of the image texture
         * @param height The height of the image texture
         * @param channels The number of color channels of the image texture
         * @param pixels The pixels of the texture
         */
        Rendering::Assets::Texture& GetTexture(const std::string& path, unsigned int type, int& width, int& height,
                                               int& channels, unsigned char*& pixels);

        /**
         * @brief Get the textures from a series of paths from a cubemap
         * @param paths The paths of a cubemap's faces
         * @return A vector of textures for each face of a cubemap
         */
        std::vector<Rendering::Assets::Texture> GetTextures(std::vector<std::string>&& paths);

        /**
         * @brief Import a texture based on it's filepath.
         * @param path The path of the texture
         * @param width Output width of the texture
         * @param height Output height of the texture
         * @param channels Output number of color channels the texture contains (1 = R, 2 = RG, 3 = RGB, 4 = RGBA). Also
         * known as bytes-per-pixel @param pixels output pixel data of the image/texture
         */
        void ImportTexture(const std::string& path, int& width, int& height, int& channels, unsigned char*& pixels);
        /**
         * @brief Import an embedded image texture
         * @param data The image buffer address.
         * @param length The size of the buffer.
         * @param width The output width of the texture
         * @param height The output height of the texture
         * @param channels Output number of color channels the texture contains (1 = R, 2 = RG, 3 = RGB, 4 = RGBA). Also
         * known as bytes-per-pixel @param pixels output pixel data of the image/texture
         */
        void ImportTexture(const void* data, int length, int& width, int& height, int& channels,
                           unsigned char*& pixels); // memory location of embedded data

        /**
         * @brief Releases a texture from memory
         * @param pixels The pixel data of a texture
         */
        void FreeTexture(void* pixels);

      private:
        /**
         * @brief Get the error texture
         * @param type The type of texture
         * @return The Error texture
         */
        Rendering::Assets::Texture& GetErrorTexture(unsigned int type);

        /**
         * @brief The path of the error texture
         */
        std::string ERROR_TEXTURE_PATH;
    };
} // namespace Vakol
