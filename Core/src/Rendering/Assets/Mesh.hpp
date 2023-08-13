#pragma once

#include <Math/Math.hpp>

#include "Rendering/RenderData.hpp"

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
        const char* name = nullptr;

        Math::Mat4 offset {};

        struct Weight
        {
            int vertexID = -1;
            float weight = 0.0f;
        };

        std::vector<Weight> weights;
    };

    struct Mesh
    {
        const char* name = nullptr;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        std::vector<Bone> bones;

        Bounds bounds{};
    };
}