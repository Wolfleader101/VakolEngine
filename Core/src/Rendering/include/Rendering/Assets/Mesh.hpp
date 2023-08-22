#pragma once

#include "Math/Math.hpp"

#include "Material.hpp"
#include "Rendering/RenderData.hpp"

#include <memory>
#include <vector>

namespace Vakol::Rendering::Assets
{
    struct Bounds
    {
        Math::Vec3 min;
        Math::Vec3 max;
    };

    struct Bone
    {
        std::string name;

        Math::Mat4 offset{};

        struct Weight
        {
            int vertexID = -1;
            float weight = 0.0f;
        };

        std::vector<Weight> weights;
    };

    struct Mesh
    {
        std::string ID = "null";

        std::string name;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        std::vector<Bone> bones;

        std::shared_ptr<Material> material = nullptr;

        Bounds bounds{};
    };
} // namespace Vakol::Rendering::Assets