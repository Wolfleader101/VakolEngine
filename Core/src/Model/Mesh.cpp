#include "Mesh.hpp"

#include "GLMesh.hpp"

namespace Vakol::Model
{
    std::shared_ptr<VertexArray> VertexArray::Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        return std::make_shared<GLVertexArray>(vertices, indices);
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(const std::vector<Vertex>& vertices)
    {
        return std::make_shared<GLVertexBuffer>(vertices);
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(const std::vector<unsigned int>& indices)
    {
        return std::make_shared<GLIndexBuffer>(indices);
    }
}

