#pragma once

#include <string>
#include <vector>

namespace Vakol::Controller
{
    /**
     * @brief Load a texture from multiple faces.
     *
     * @param faces A vector of file paths for the texture faces.
     * @param gamma Flag indicating whether gamma correction should be applied.
     * @param flip Flag indicating whether the texture should be flipped vertically.
     * @return unsigned int The ID of the loaded texture.
     */
    unsigned int LoadTexture(std::vector<const char *> &&faces, bool gamma, bool flip);

    /**
     * @brief Load a texture with specified parameters.
     *
     * @param size The size of the texture.
     * @param gamma Flag indicating whether gamma correction should be applied.
     * @param flip Flag indicating whether the texture should be flipped vertically.
     * @param data Pointer to additional data for the texture.
     * @return unsigned int The ID of the loaded texture.
     */
    unsigned int LoadTexture(int size, bool gamma, bool flip, const void *data);

    /**
     * @brief Load a texture from a file.
     *
     * @param path The file path of the texture.
     * @param gamma Flag indicating whether gamma correction should be applied.
     * @param flip Flag indicating whether the texture should be flipped vertically.
     * @return unsigned int The ID of the loaded texture.
     */
    unsigned int LoadTexture(std::string &path, bool gamma, bool flip);

    /**
     * @brief Load a raw texture from a file.
     *
     * @param path The file path of the raw texture.
     * @return unsigned int The ID of the loaded texture.
     */
    unsigned int LoadRawTexture(std::string &path);
} // namespace Vakol::Controller