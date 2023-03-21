#pragma once

#include "Buffer.hpp"

namespace Vakol::Model {
    class Model {};

    class Mesh {
       public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
            : vertices(vertices), indices(indices){};
        ~Mesh() = default;

        virtual void Draw(const unsigned int mode) const = 0;
        const virtual unsigned int GetID() const = 0;

        const std::vector<Vertex>& GetVertices() const { return vertices; }

       protected:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };
}  // namespace Vakol::Model