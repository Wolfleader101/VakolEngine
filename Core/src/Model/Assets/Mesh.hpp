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

        /// @brief 
        /// @param mode the mode of drawing: DEFAULT, INSTANCED, STRIPS, PATCHES
        void SetDrawMode(const DRAW_MODE mode) { this->m_vertexArray->set_mode(mode); }
        void SetDrawModeInfo(const unsigned int info) { this->m_vertexArray->set_mode_data(info); }

        /// @brief 
        /// @param type the type of drawing method: ARRAYS, ELEMENTS
        void SetDrawType(const DRAW_TYPE type) { this->m_vertexArray->set_type(type); }
        
        // call this if you are drawing with strips
        void SetStripInfo(const unsigned int info) { this->m_vertexArray->set_strip_data(info); }

        void Draw() const { this->m_vertexArray->Draw(); }
    private:
        std::shared_ptr<VertexArray> m_vertexArray;
        std::shared_ptr<Material> m_material;
    };
}