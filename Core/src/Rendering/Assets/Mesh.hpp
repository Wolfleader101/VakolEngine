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

    struct Mesh
    {
        const char* name = nullptr;

        std::vector<Rendering::Vertex> vertices;
        std::vector<unsigned int> indices;

        Bounds bounds{};
    };
}