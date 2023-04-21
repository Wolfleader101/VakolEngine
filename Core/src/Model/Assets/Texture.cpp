#include "Texture.hpp"

#include <glad/glad.h>

namespace Vakol::Model::Assets
{
    void Texture::Bind(const unsigned int slot) const
    {
        glBindTextureUnit(slot, this->m_ID);
    }

    void Texture::Unbind(const unsigned int slot) const
    {
        glBindTextureUnit(slot, 0);
    }
}
