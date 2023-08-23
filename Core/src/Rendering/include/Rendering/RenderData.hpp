#pragma once

#include <Math/Math.hpp>

#include <string>
#include <vector>

constexpr int MAX_BONE_INFLUENCE = 4;

namespace Vakol::Rendering
{
    struct Vertex
    {
        Math::Vec3 position;
        Math::Vec3 normal;
        Math::Vec2 uv;
        Math::Vec3 tangent;
        Math::Vec3 bitangent;

        int boneIDs[MAX_BONE_INFLUENCE];       ///< Array of bone IDs affecting the vertex.
        float boneWeights[MAX_BONE_INFLUENCE]; ///< Array of weights of the bones affecting the vertex.
    };

    struct Drawable
    {
        bool active = true;

        std::string modelID = "null";
        std::string shaderID = "null";
    };

    struct Skybox
    {
        bool active = false;

        std::string vertexID = "null";
        std::string shaderID = "null";

        unsigned int textureID = 0;
    };

    struct VertexArray
    {
        std::string ID = "null";

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    struct SkyboxVertexArray
    {
        std::string ID = "null";

        std::vector<float> vertices;
    };

    struct Uniform
    {
        int location;
        int count; // number of locations the uniform variable takes up in memory.
    };

    struct UniformBuffer
    {
        unsigned int id;
        unsigned int binding;
    };
} // namespace Vakol::Rendering
