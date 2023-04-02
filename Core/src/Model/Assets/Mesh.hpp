#pragma once

#include "Material.hpp"


namespace Vakol::Model::Assets
{
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    struct Mesh {
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material)
            : vertices(vertices), indices(indices), material(material) {}

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        Material material;
    };
}