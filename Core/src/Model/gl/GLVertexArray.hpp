#pragma once

#include <Model/VertexArray.hpp>

namespace Vakol::Model {
    class GLVertexArray : public VertexArray {
       public:
        GLVertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        GLVertexArray(const std::vector<float>& vertices);
        GLVertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

        ~GLVertexArray();

        void GenArray(const unsigned int n, unsigned int* array);
        void GenBuffer(const unsigned int n, unsigned int* buffer);

        void BindBuffer(const unsigned int type, const unsigned int buffer);

        void Bind() const override;
        void Unbind() const override;

        void DrawArrays() const override;
        void DrawElements() const override;

        void DrawTriangleStrips() const override;
        void DrawQuadPatches() const override;

        void DrawArraysInstanced(const int amount) const override;
        void DrawElementsInstanced(const int amount) const override;

        const unsigned int GetID() const override { return this->VAO_ID; }

        const int GetVertexCount() const override { return static_cast<int>(this->m_vertices.size()); }
        const int GetIndexCount() const override { return static_cast<int>(this->m_indices.size()); }

        const std::vector<Vertex> GetVertices() const override { return this->m_vertices; }
        const std::vector<float> GetFloatVertices() const override { return this->m_verts; }
        const std::vector<unsigned int>& GetIndices() const override { return this->m_indices; }

        void SetStrips(const int strips, const int tris) override {
            this->NUM_STRIPS = strips;
            this->NUM_TRIS_PER_STRIP = tris;
        }

        void SetPatches(const int num_patches, const int num_verts_per_patch) override {
            this->NUM_PATCHES = num_patches;
            this->NUM_VERTS_PER_PATCH = num_verts_per_patch;
        }

        void SetDrawMode(const unsigned char mode) { this->DRAW_MODE = mode; }

        const unsigned char GetDrawMode() const { return this->DRAW_MODE; }

       private:
        unsigned int VAO_ID = 0;
        unsigned int VBO_ID = 0;
        unsigned int EBO_ID = 0;
    };
}  // namespace Vakol::Model