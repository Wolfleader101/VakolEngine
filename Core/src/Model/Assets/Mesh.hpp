#pragma once

#include <Controller/Logger.hpp>
#include <Model/Assets/Material.hpp>
#include <Model/VertexArray.hpp>
#include <memory>

namespace Vakol::Model::Assets {
    class Mesh {
       public:
        Mesh() = default;

        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const size_t size,
             const MaterialSpec& spec = DEFAULT)
            : m_vertex_array(std::make_shared<VertexArray>(std::move(vertices), std::move(indices), size)),
              m_material(std::make_shared<Material>(spec)) {}

        Mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, const size_t size,
             const MaterialSpec& spec = DEFAULT)
            : m_vertex_array(std::make_shared<VertexArray>(Convert(vertices, size), std::move(indices), size)),
              m_material(std::make_shared<Material>(spec)) {}

        unsigned int GetId() const { return this->m_vertex_array->GetId(); }

        void SetMaterial(MaterialSpec& spec) {
            if (!this->m_material) this->m_material = std::make_shared<Material>(std::move(spec));
        }
        void SetMaterial(MaterialSpec&& spec) {
            if (!this->m_material) this->m_material = std::make_shared<Material>(std::move(spec));
        }

        const std::shared_ptr<Material>& GetMaterial() const { return this->m_material; }

        const std::vector<Vertex>& c_vertices() const { return this->m_vertex_array->GetConstVertices(); }

        std::vector<Vertex>& vertices() const { return this->m_vertex_array->GetVertices(); }
        const std::vector<unsigned int>& indices() const { return this->m_vertex_array->GetIndices(); }

        int nVertices() const { return this->m_vertex_array->GetVertexCount(); }
        int nIndices() const { return this->m_vertex_array->GetIndexCount(); }

        /// @brief .
        /// @param mode the mode of drawing: DEFAULT, INSTANCED, STRIPS, PATCHES
        void SetDrawMode(const DRAW_MODE mode) const { this->m_vertex_array->set_mode(mode); }
        void SetDrawModeInfo(const int info) const { this->m_vertex_array->set_mode_data(info); }

        /// @brief .
        /// @param type the type of drawing method: ARRAYS, ELEMENTS
        void SetDrawType(const DRAW_TYPE type) const { this->m_vertex_array->set_type(type); }

        // (size / 1) * 2 - 2
        void SetNumTrisPerStrip(const int info) const { this->m_vertex_array->set_num_tris_per_strip(info); }

        void Draw() const { this->m_vertex_array->Draw(); }

       private:
        std::shared_ptr<VertexArray> m_vertex_array;

        std::shared_ptr<Material> m_material;
    };
}  // namespace Vakol::Model::Assets