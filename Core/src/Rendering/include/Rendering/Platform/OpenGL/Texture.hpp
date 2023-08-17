#pragma once

namespace Vakol::Rendering::OpenGL
{
    unsigned int GenerateTexture(int width, int height, int channels, const unsigned char* pixels);

    void SetActiveTexture(int slot);
    void BindTexture(unsigned int texture);
} // namespace Vakol::Rendering::OpenGL