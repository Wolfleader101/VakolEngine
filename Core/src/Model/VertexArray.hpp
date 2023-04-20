#pragma once

#include <Model/Assets/Vertex.hpp>
#include <vector>

namespace Vakol::Model {
    using Assets::Vertex;

    class VertexArray 
    {
    public:
        VertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : m_vertices(vertices), m_indices(indices) {}
        VertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) : m_verts(vertices), m_indices(indices) {};
        VertexArray(const std::vector<float>& vertices) : m_verts(vertices) {};

        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void DrawArrays() const = 0;
        virtual void DrawElements() const = 0;

        virtual void DrawArraysInstanced(const int amount) const = 0;
        virtual void DrawElementsInstanced(const int amount) const = 0;

        virtual void DrawTriangleStrips() const = 0;
        virtual void DrawQuadPatches() const = 0;

        const virtual unsigned int GetID() const = 0;
        const virtual int GetVertexCount() const = 0;
        const virtual int GetIndexCount()  const = 0;

        virtual const std::vector<Vertex> GetVertices() const = 0;
        virtual const std::vector<unsigned int> GetIndices() const = 0;

        virtual void SetStrips(const int strips, const int tris) = 0;
        virtual void SetPatches(const int num_patches, const int num_verts_per_patch) = 0;

    protected:
        std::vector<Vertex> m_vertices;
        std::vector<float> m_verts;

        std::vector<unsigned int> m_indices;

        int NUM_STRIPS = 0;
        int NUM_TRIS_PER_STRIP = 0;

        int NUM_PATCHES = 0;
        int NUM_VERTS_PER_PATCH = 0;
    };
}
