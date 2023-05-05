#pragma once

#include <Model/Assets/Material.hpp>
#include <Model/VertexArray.hpp>

#include <memory>

namespace Vakol::Model::Assets 
{
    class Mesh 
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const int size, const MaterialSpec& spec = DEFAULT)
            : m_vertexArray(std::make_shared<VertexArray>(Convert(vertices, size), std::move(indices), size)), m_material(std::make_shared<Material>(spec)) {};

        Mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, const int size, const MaterialSpec& spec = DEFAULT)
            : m_vertexArray(std::make_shared<VertexArray>(std::move(vertices), std::move(indices), size)), m_material(std::make_shared<Material>(spec)) {};

        void SetMaterial(const MaterialSpec& spec) { if (!this->m_material) this->m_material = std::make_shared<Material>(spec); }

        [[nodiscard]] unsigned int GetId() const { return this->m_vertexArray->GetId(); }

		[[nodiscard]] const std::shared_ptr<Material>& GetMaterial() const { return this->m_material; }

        [[nodiscard]] const std::vector<float>& GetVertices() const { return this->m_vertexArray->GetVertices(); }

        /// @brief .
        /// @param mode the mode of drawing: DEFAULT, INSTANCED, STRIPS, PATCHES
        void SetDrawMode(const DRAW_MODE mode) const { this->m_vertexArray->set_mode(mode); }
        void SetDrawModeInfo(const int info) const { this->m_vertexArray->set_mode_data(info); }

        /// @brief .
        /// @param type the type of drawing method: ARRAYS, ELEMENTS
        void SetDrawType(const DRAW_TYPE type) const { this->m_vertexArray->set_type(type); }
        
        // (size / 1) * 2 - 2
        void SetNumTrisPerStrip(const int info) const { this->m_vertexArray->set_num_tris_per_strip(info); }

        void Draw() const { this->m_vertexArray->Draw(); }
    private:
        std::shared_ptr<VertexArray> m_vertexArray;
        std::shared_ptr<Material> m_material;
    };
}