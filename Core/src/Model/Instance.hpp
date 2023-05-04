#pragma once

#include <Model/Assets/Mesh.hpp>

namespace Vakol::Model
{
    using Vakol::Model::Assets::Mesh;

    void CreateInstances(const std::vector<Mesh>& meshes, const std::vector<glm::mat4>& matrices);
}