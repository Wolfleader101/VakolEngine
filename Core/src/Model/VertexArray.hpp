#pragma once

#include <Model/Assets/Vertex.hpp>
#include <vector>

namespace Vakol::Model {
    using Assets::Vertex;

    class VertexArray {
       public:
        VertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
            : vertices(vertices), indices(indices) {}
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void DrawArrays() const = 0;
        virtual void DrawElements() const = 0;

        virtual void DrawArraysInstanced(const int amount) const = 0;
        virtual void DrawElementsInstanced(const int amount) const = 0;
        virtual void DrawElementsStripped(unsigned int strips) const = 0;

        const virtual unsigned int GetID() const = 0;
        const virtual unsigned int GetVertices() const = 0;
        const virtual unsigned int GetIndices() const = 0;

        const std::vector<Vertex>& GetVerticeVec();
        const std::vector<unsigned int> GetIndiceVec();

       protected:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };
}  // namespace Vakol::Model
