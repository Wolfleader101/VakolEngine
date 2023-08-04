#pragma once

#include "Mesh.hpp"

namespace Vakol::Rendering::Assets
{
    struct Model
    {
        const char* name = nullptr;

        std::vector<Mesh> meshes;
    };
}