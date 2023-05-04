#pragma once

#include <Model/Assets/Material.hpp>
#include <Model/VertexArray.hpp>

#include <memory>

namespace Vakol::Model::Assets 
{

    class Mesh 
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const int size, const MaterialSpec& spec = DEFAULT)
            : m_vertexArray(std::make_shared<VertexArray>(Convert(vertices, size), indices, size)), m_material(std::make_shared<Material>(spec)) {};

        Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const int size, const MaterialSpec& spec = DEFAULT)
            : m_vertexArray(std::make_shared<VertexArray>(vertices, indices, size)), m_material(std::make_shared<Material>(spec)) {};

        void SetMaterial(const MaterialSpec& spec) { if (!this->m_material) this->m_material = std::make_shared<Material>(spec); }

        const unsigned int GetID() const { return this->m_vertexArray->GetID(); }

        const std::shared_ptr<Material>& GetMaterial() const { return this->m_material; }

        const std::vector<float>& GetVertices() const { return this->m_vertexArray->GetVertices(); }
        // const std::shared_ptr<VertexArray>& GetVertexArray() const { return this->m_vertexArray; }
        void Draw() const { this->m_vertexArray->Draw(); }
    private:
        std::shared_ptr<VertexArray> m_vertexArray;
        std::shared_ptr<Material> m_material;
    };
}