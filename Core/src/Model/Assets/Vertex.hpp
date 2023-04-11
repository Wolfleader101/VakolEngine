#pragma once 

#include <glm/common.hpp>
#include <glm/glm.hpp>

namespace Vakol::Model::Assets
{
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };
}
