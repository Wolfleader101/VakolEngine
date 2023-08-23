#pragma once

#include <string>
#include <vector>

namespace Vakol::Rendering::Assets
{
    struct Texture;
}

namespace Vakol::Rendering::OpenGL
{
    /**
     * \brief Generates a Texture via OpenGL API calls, submitting data to GPU.
     * \param levels the numbers of mipmap levels the texture should have (default is 1)
     * \param width the width of the texture
     * \param height the height of the texture
     * \param channels the number of color channels of the texture.
     * \param pixels the pixel data of the texture
     * \return the texture id (return 0 = fail)
     */
    unsigned int GenerateTexture(int levels, int width, int height, int channels, const unsigned char* pixels);
    /**
     * \brief Generate a cubemap Texture via OpenGL API calls, submitting data to GPU.
     * \param textures pre-loaded textures generated by assetloader
     * \return the texture id (return 0 = fail)
     */
    unsigned int GenerateTexture(std::vector<Assets::Texture>&& textures);

    /**
     * \brief Sets the active texture within an OpenGL context
     * \param slot added to the active slot (i.e. GL_TEXTURE0 + slot)
     */
    void SetActiveTexture(int slot);

    /**
     * \brief binds a texture within an OpenGL context.
     * \param texture the unique texture id generated by OpenGL.
     */
    void BindTexture(unsigned int texture);
    /**
     * \brief binds a cubemap texture within an OpenGL context.
     * \param texture the unique cubemap texture id generated by OpenGL.
     */
    void BindCubemapTexture(unsigned int texture);
} // namespace Vakol::Rendering::OpenGL