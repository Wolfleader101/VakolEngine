#pragma once

namespace Vakol::Rendering::OpenGL
{
    unsigned int GenerateTexture(int width, int height, const unsigned char* pixels);

    void SetActiveTexture(int slot);
    void BindTexture(unsigned int texture);
}