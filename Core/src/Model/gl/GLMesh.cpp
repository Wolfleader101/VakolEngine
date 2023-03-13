#include <glad/glad.h>

#include <Controller/Logger.hpp>

#include "GLMesh.hpp"

namespace Vakol::Model
{
    GLMesh::GLMesh(const GLVertexArray& VAO)
    {
        this->VAO = VAO;
    }

    GLMesh::GLMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        this->VAO = GLVertexArray(vertices, indices);
    }

    const unsigned int GLMesh::GetSize(const unsigned int type) const
    {
        if (type == GL_VERTEX_ARRAY)
            return VAO.GetVertexCount();
        else if (type == GL_INDEX_ARRAY)
            return VAO.GetIndexCount();
        else
            VK_ERROR("ERROR:: Incorrect Size Type\n\n\t   Returning 0");
        return 0;
    }


    void GLMesh::Draw() const
    {
        VAO.Bind();
        glDrawElements(GL_TRIANGLES, VAO.GetIndexCount(), GL_UNSIGNED_INT, 0);
        VAO.Unbind();
    }
}