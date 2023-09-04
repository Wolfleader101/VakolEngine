#pragma once

#include "Math/Math.hpp"

#include <string>
#include <vector>

constexpr int MAX_BONE_INFLUENCE = 4; // how many bones can influence a vertex.

namespace Vakol::Rendering
{
    struct Vertex
    {
        Math::Vec3 position;  // the position of a vertex.
        Math::Vec3 normal;    // the normal of a vertex.
        Math::Vec2 uv;        // the texture coordinates of a vertex
        Math::Vec3 tangent;   // the tangent of a vertex
        Math::Vec3 bitangent; // the bitangent of a vertex.

        int boneIDs[MAX_BONE_INFLUENCE];       ///< Array of bone IDs affecting the vertex.
        float boneWeights[MAX_BONE_INFLUENCE]; ///< Array of weights of the bones affecting the vertex.
    };

    struct DebugVertex
    {
        Math::Vec3 position;
        Math::Vec3 color;
    };

    struct Drawable
    {
        bool active = true; /// decides whether the drawable should be drawn in the scene.

        std::string ID = "null";       // a unique id of the drawable
        std::string shaderID = "null"; // A unique id of the shader.
    };

    struct Skybox
    {
        bool active = false; /// decides whether the skybox should be drawn in the scene.

        std::string vertexID = "null";
        std::string shaderID = "null";

        unsigned int textureID = 0;
    };

    struct DebugScene
    {
        std::string ID = "null";
        std::string shaderID = "null";
    };

    struct VertexArray
    {
        std::string ID = "null"; // the unique id of the vertex array.

        std::vector<Vertex> vertices;      // the vertices of a vertex array.
        std::vector<unsigned int> indices; // the indices of a vertex array.
    };

    struct SkyboxVertexArray
    {
        std::string ID = "null"; // the unique id of the skybox vertex array

        std::vector<float> vertices; // the vertices of a skybox.
    };

    struct Uniform
    {
        int location; // the location of the uniform variable within the shader
        int count;    // number of locations the uniform variable takes up in memory.
    };

    struct UniformBuffer
    {
        unsigned int id;      // the unique id of the uniform buffer
        unsigned int binding; // the binding uniform location of the uniform buffer.
    };
} // namespace Vakol::Rendering