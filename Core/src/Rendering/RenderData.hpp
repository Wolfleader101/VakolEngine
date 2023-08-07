#pragma once

#include <Math/Math.hpp>

#include <vector>
#include <string>

namespace Vakol::Rendering
{
    enum VK_TEXTURE_TYPE
    {
        VK_TEXTURE_NONE,
        VK_TEXTURE_AMBIENT,
        VK_TEXTURE_DIFFUSE,
        VK_TEXTURE_SPECULAR,
        VK_TEXTURE_NORMAL,
        VK_TEXTURE_EMISSION
    };

    struct Vertex
    {
        Math::Vec3 position;
        Math::Vec3 normal;
        Math::Vec2 uv;
        Math::Vec3 tangent;
        Math::Vec3 bitangent;
    };

    struct VertexArray
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    struct Shader
    {
        const char* path = nullptr;
    };

    struct Texture
    {
        const char* path = nullptr;

        int width = 0;
        int height = 0;
        int bpp = 0; // bytes-per-pixel (number of color channels in an image) R = 1, RG = 2, RGB = 3, RGBA = 4

        VK_TEXTURE_TYPE type = VK_TEXTURE_NONE;
    };
}
