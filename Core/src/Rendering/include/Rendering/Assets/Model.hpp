#pragma once

#include "Mesh.hpp"

namespace Vakol::Rendering::Assets
{
    struct Model
    {
        std::string name;

        std::vector<Mesh> meshes;
    };
}