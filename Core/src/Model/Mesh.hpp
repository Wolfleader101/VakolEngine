#pragma once

#include <vector>

#include "Buffer.hpp"
#include "Texture.hpp"

namespace Vakol::Model
{
    const unsigned int DRAW_MODE_INDEXED = 0;
    const unsigned int DRAW_MODE_NON_INDEXED = 1;

    class Mesh
    {
    public:
        ~Mesh() = default;

        virtual void Draw(const unsigned int mode) const = 0;
        const virtual unsigned int GetID() const = 0;
    };
}