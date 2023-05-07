#pragma once

#include <Model/Assets/Material.hpp>
#include <Model/VertexArray.hpp>

#include <memory>
#include <Controller/Logger.hpp>

namespace Vakol::Model::Assets 
{
    struct Bone
    {
        std::string name;
        int index = -1;
        glm::mat4 offset;
    };

    class Mesh 
    {
    public:
        Mesh() = default;

        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, MaterialSpec&& spec = std::move(DEFAULT))
            : m_vertex_array(std::make_shared<VertexArray>(Convert(vertices), std::move(indices), static_cast<int>(sizeof(Vertex)))), m_material(std::make_shared<Material>(std::move(spec))) {}

        Mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, const int size, MaterialSpec&& spec = std::move(DEFAULT))
            : m_vertex_array(std::make_shared<VertexArray>(std::move(vertices), std::move(indices), size)), m_material(std::make_shared<Material>(std::move(spec))) {}

        [[nodiscard]] unsigned int GetId() const { return this->m_vertex_array->GetId(); }

        void SetMaterial(MaterialSpec& spec) { if (!this->m_material) this->m_material = std::make_shared<Material>(std::move(spec)); }
        void SetMaterial(MaterialSpec&& spec) { if (!this->m_material) this->m_material = std::make_shared<Material>(std::move(spec)); }

        [[nodiscard]] const std::shared_ptr<Material>& GetMaterial() const { return this->m_material; }

        void set(std::vector<Bone>& in_bones) { this->bones = std::move(in_bones); }
        void set(std::unordered_map<std::string, int>& map) { this->bone_map = std::move(map); }

        [[nodiscard]] const std::vector<float>& const_vertices() const { return this->m_vertex_array->GetConstVertices(); }

        [[nodiscard]] std::vector<float>& vertices() const { return this->m_vertex_array->GetVertices(); }
        [[nodiscard]] const std::vector<unsigned int>& indices() const { return this->m_vertex_array->GetIndices(); }

        [[nodiscard]] int nVertices() const { return this->m_vertex_array->GetVertexCount(); }
        [[nodiscard]] int nIndices() const { return this->m_vertex_array->GetIndexCount(); }

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

        std::vector<Bone> bones;
        std::unordered_map<std::string, int> bone_map;
    };
}