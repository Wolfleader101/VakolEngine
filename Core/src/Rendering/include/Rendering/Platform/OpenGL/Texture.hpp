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
     * \brief 
     * \param levels 
     * \param width 
     * \param height 
     * \param channels 
     * \param pixels 
     * \return 
     */
    unsigned int GenerateTexture(int levels, int width, int height, int channels, const unsigned char* pixels);
    /**
     * \brief 
     * \param textures 
     * \return 
     */
    unsigned int GenerateTexture(std::vector<Assets::Texture>&& textures);

    /**
     * \brief 
     * \param slot 
     */
    void SetActiveTexture(int slot);

    /**
     * \brief 
     * \param texture 
     */
    void BindTexture(unsigned int texture);
    /**
     * \brief 
     * \param texture 
     */
    void BindCubemapTexture(unsigned int texture);
} // namespace Vakol::Rendering::OpenGL