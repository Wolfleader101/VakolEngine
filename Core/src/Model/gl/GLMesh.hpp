#pragma once

#include "GLBuffer.hpp"

#include "../Mesh.hpp"

namespace Vakol::Model
{
    class GLMesh : public Mesh
    {
    public:
        GLMesh(const GLVertexArray& VAO);
        GLMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        const unsigned int GetSize(const unsigned int type) const;

        void Draw(const unsigned int mode) const override;

        const unsigned int GetID() const override;
    private:
        GLVertexArray VAO;
    };
}