#pragma once

#include "Material.hpp"

#include <Model/Assets/Vertex.hpp>

#include <Model/gl/GLVertexArray.hpp>

#include <memory>


namespace Vakol::Model::Assets
{
    class Mesh 
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material)
            : m_vertexArray(std::make_shared<GLVertexArray>(vertices, indices)), m_material(material) {}

        const Material material() const { return this->m_material; }
        const std::shared_ptr<VertexArray> vao() const { return this->m_vertexArray; }

    private:
        std::shared_ptr<VertexArray> m_vertexArray;
        Material m_material;
    };
}