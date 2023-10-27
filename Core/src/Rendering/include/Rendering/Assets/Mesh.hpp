#pragma once

#include "Math/Math.hpp"

#include "Material.hpp"
#include "Rendering/RenderData.hpp"

#include <memory>
#include <vector>

namespace Vakol::Rendering::Assets
{
    /**
     * \brief AABB Bounds
     */
    struct Bounds
    {
        /**
         * \brief min bounds
         */
        Math::Vec3 min;

        /**
         * \brief max bounds
         */
        Math::Vec3 max;

        /**
         * \brief the half extents
         */
        Math::Vec3 halfExtents;
    };

    struct Bone
    {
        std::string name; // the name of the bone

        Math::Mat4 offset{}; // the offset of the bone

        struct Weight
        {
            int vertexID = -1;
            float weight = 0.0f;
        };

        std::vector<Weight> weights;
    };

    struct Mesh
    {
        std::string ID; // the unique ID of the mesh.

        std::string name; // the name of the mesh.

        std::vector<Vertex> vertices;      // the vertices of a mesh.
        std::vector<unsigned int> indices; // the indices of a mesh.

        std::vector<Bone> bones; // the bones of a mesh.

        Material material; // the material of a mesh.

        Bounds bounds; // the AABB bounds of a mesh.
    };
} // namespace Vakol::Rendering::Assets