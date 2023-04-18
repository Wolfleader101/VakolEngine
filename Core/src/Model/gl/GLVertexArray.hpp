#pragma once

#include <Model/VertexArray.hpp>

namespace Vakol::Model {
    class GLVertexArray : public VertexArray {
       public:
        GLVertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~GLVertexArray();

        void GenArray(const unsigned int n, unsigned int* array);
        void GenBuffer(const unsigned int n, unsigned int* buffer);

        void BindBuffer(const unsigned int type, const unsigned int buffer);

        void Bind() const override;
        void Unbind() const override;

        void DrawArrays() const override;
        void DrawElements() const override;
        void DrawTriangleStrips(int strips) const override;

        void DrawArraysInstanced(const int amount) const override;
        void DrawElementsInstanced(const int amount) const override;

        const unsigned int GetID() const override { return this->VAO_ID; }

        const int GetVertexCount() const override { return static_cast<int>(this->m_vertices.size()); }
        const int GetIndexCount() const override { return static_cast<int>(this->m_indices.size()); }

        const std::vector<Vertex> GetVertices() const override { return this->m_vertices; }
        const std::vector<unsigned int> GetIndices() const override { return this->m_indices; }

    private:
        unsigned int VAO_ID = 0;
        unsigned int VBO_ID = 0;
        unsigned int EBO_ID = 0;
    };
}