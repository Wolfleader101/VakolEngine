#pragma once

#include <Model/Assets/Vertex.hpp>
#include <vector>

namespace Vakol::Model {
    using Assets::Vertex;

    class VertexArray 
    {
    public:
        VertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : m_vertices(vertices), m_indices(indices) {}
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void DrawArrays() const = 0;
        virtual void DrawElements() const = 0;

        virtual void DrawArraysInstanced(const int amount) const = 0;
        virtual void DrawElementsInstanced(const int amount) const = 0;

        virtual void DrawTriangleStrips(const int strips, const int tris) const = 0;

        const virtual unsigned int GetID() const = 0;
        const virtual int GetVertexCount() const = 0;
        const virtual int GetIndexCount()  const = 0;

        virtual const std::vector<Vertex> GetVertices() const = 0;
        virtual const std::vector<unsigned int> GetIndices() const = 0;

    protected:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
    };
}
