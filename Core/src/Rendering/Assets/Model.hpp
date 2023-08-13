#pragma once

#include "Mesh.hpp"
#include "Material.hpp"

namespace Vakol::Rendering::Assets
{
    struct Model
    {
        const char* name = nullptr;

        std::vector<Mesh> meshes;
        std::vector<Material> materials;
    };
}