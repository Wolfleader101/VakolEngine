#pragma once

#include <Math/Math.hpp>

#include "Rendering/RenderData.hpp"

#include <vector>

namespace Vakol::Rendering::Assets
{
    struct Mesh
    {
    private:
        struct Bounds
        {
            Math::Vec3 min;
            Math::Vec3 max;
        };

    public:
        const char* name = nullptr;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        Bounds bounds;
    };
}