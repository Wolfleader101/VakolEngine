#pragma once

#include <vector>
#include <string>


#include "Mesh.hpp"

namespace Vakol::Model::Assets {

    struct Model {
        Model() = default;
        Model(const std::vector<Mesh>& meshes) : meshes(meshes){};

        std::vector<Mesh> meshes;
    };

}  // namespace Vakol::Model::Assets