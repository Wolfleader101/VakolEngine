#pragma once

#include "Math/include/Math.hpp"
#include <Rendering/include/Assets/Mesh.hpp>

namespace Vakol
{
    using Assets::Mesh;

    /**
     * @brief Function to create instances.
     * @param meshes Vector of Meshes to be instantiated.
     * @param matrices Vector of transformation matrices for the instances.
     * @param amount The number of instances to create.
     * @param start_index The starting index for instance creation. Default is 3.
     */
    void CreateInstances(const std::vector<Mesh>& meshes, const std::vector<Math::Mat4>& matrices, int amount,
                         int start_index = 3);
} // namespace Vakol