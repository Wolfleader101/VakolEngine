#pragma once

#include "Material.hpp"

#include <Model/Assets/Vertex.hpp>


namespace Vakol::Model::Assets
{

    struct Mesh {
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material)
            : vertices(vertices), indices(indices), material(material) {}

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        Material material;
    };
}