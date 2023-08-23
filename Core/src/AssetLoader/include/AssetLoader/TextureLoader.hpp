#pragma once

#include <string>

namespace Vakol
{
    /**
     * \brief
     * \param path the path of the texture
     * \param width output width of the texture
     * \param height output height of the texture
     * \param channels output number of color channels the texture contains (1 = R, 2 = RG, 3 = RGB, 4 = RGBA). Also
     * known as bytes-per-pixel \param pixels output pixel data of the image/texture
     */
    void ImportTexture(const std::string& path, int& width, int& height, int& channels, unsigned char*& pixels);
    /**
     * \brief
     * \param data
     * \param length
     * \param width
     * \param height
     * \param channels output number of color channels the texture contains (1 = R, 2 = RG, 3 = RGB, 4 = RGBA). Also
     * known as bytes-per-pixel \param pixels output pixel data of the image/texture
     */
    void ImportTexture(const void* data, int length, int& width, int& height, int& channels,
                       unsigned char*& pixels); // memory location of embedded data
} // namespace Vakol