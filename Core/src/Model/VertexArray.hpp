#pragma once

#include <Model/Assets/Vertex.hpp>
#include <vector>

const unsigned char ARRAYS = 0x0;
const unsigned char ELEMENTS = 0x1;

const unsigned char ARRAYS_INSTANCED = 0x2;
const unsigned char ELEMENTS_INSTANCED = 0x3;

const unsigned char TRIANGLE_STRIPS = 0x4;
const unsigned char QUAD_PATCHES = 0x5;

namespace Vakol::Model {
    using Assets::Vertex;

    class VertexArray {
       public:
        VertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
            : m_vertices(vertices), m_indices(indices) {}
        VertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
            : m_verts(vertices), m_indices(indices){};
        VertexArray(const std::vector<float>& vertices) : m_verts(vertices){};

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
        const virtual int GetIndexCount() const = 0;

        virtual const std::vector<Vertex> GetVertices() const = 0;
        virtual const std::vector<float> GetFloatVertices() const = 0;
        virtual const std::vector<unsigned int>& GetIndices() const = 0;

        virtual void SetStrips(const int strips, const int tris) = 0;
        virtual void SetPatches(const int num_patches, const int num_verts_per_patch) = 0;

        virtual void SetDrawMode(const unsigned char mode) = 0;
        virtual const unsigned char GetDrawMode() const = 0;

       protected:
        std::vector<Vertex> m_vertices;
        std::vector<float> m_verts;

        std::vector<unsigned int> m_indices;

        unsigned char DRAW_MODE = ELEMENTS;

        int NUM_STRIPS = 0;
        int NUM_TRIS_PER_STRIP = 0;

        int NUM_PATCHES = 0;
        int NUM_VERTS_PER_PATCH = 0;
    };
}  // namespace Vakol::Model
