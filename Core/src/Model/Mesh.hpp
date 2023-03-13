#pragma once

#include <vector>

#include "Buffer.hpp"
#include "Texture.hpp"

namespace Vakol::Model
{
    class Mesh
    {
    public:
        ~Mesh() = default;

        virtual void Draw() const = 0;
    };
}