#pragma once

#include "Mesh.hpp"

namespace Vakol::Rendering::Assets
{
    struct Model
    {
        std::string path; /// the path of the model

        std::string name; /// the name of the model

        std::vector<Mesh> meshes; /// the meshes of a model
    };
} // namespace Vakol::Rendering::Assets
