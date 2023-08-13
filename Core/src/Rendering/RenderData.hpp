#pragma once

#include <Math/Math.hpp>

#include <vector>
#include <string>

namespace Vakol::Rendering
{
    struct Vertex
    {
        Math::Vec3 position;
        Math::Vec3 normal;
        Math::Vec2 uv;
        Math::Vec3 tangent;
        Math::Vec3 bitangent;
    };

    struct Drawable
    {
        std::string vertexArrayID = "null";
        std::string shaderID = "null";
    };

    struct VertexArray
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    struct Uniform
    {
        int location;
        int count; // number of locations the uniform variable takes up in memory.
    };
}
