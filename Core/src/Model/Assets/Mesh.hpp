#pragma once

#include <Model/Assets/Vertex.hpp>
#include <Model/gl/GLMaterial.hpp>
#include <Model/gl/GLVertexArray.hpp>
#include <memory>

namespace Vakol::Model::Assets 
{
    class Mesh 
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const MaterialSpec& spec)
            : m_vertexArray(std::make_shared<GLVertexArray>(vertices, indices)),
              m_material(std::make_shared<GLMaterial>(spec)) {};

        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
            : m_vertexArray(std::make_shared<GLVertexArray>(vertices, indices)),
              m_material(nullptr) {};

        void SetMaterial(const MaterialSpec& spec) { if (!this->m_material) this->m_material = std::make_shared<GLMaterial>(spec); }

        const std::shared_ptr<Material>& material() const { return this->m_material; }
        const std::shared_ptr<VertexArray>& vao() const { return this->m_vertexArray; }

    private:
        std::shared_ptr<VertexArray> m_vertexArray;
        std::shared_ptr<Material> m_material;
    };
}