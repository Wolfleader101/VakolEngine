#pragma once

#include <Logger/include/Logger.hpp>
#include <Model/Assets/Material.hpp>
#include <Model/VertexArray.hpp>
#include <memory>

namespace Vakol::Model::Assets
{
    /**
     * @brief Class representing a mesh.
     */
    class Mesh
    {
      public:
        Mesh() = default;

        /**
         * @brief Constructs a Mesh object with the given vertices, indices, size, and material specifications.
         *
         * @param vertices The vertices of the mesh.
         * @param indices The indices of the mesh.
         * @param size The size of the mesh.
         * @param spec The material specifications of the mesh.
         */
        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const size_t size,
             MaterialSpec spec = DEFAULT)
            : m_vertex_array(std::make_shared<VertexArray>(std::move(vertices), std::move(indices), size)),
              m_material(std::make_shared<Material>(std::move(spec)))
        {
        }

        /**
         * @brief Constructs a Mesh object with the given vertices, indices, size, and material specifications.
         *
         * @param vertices The vertices of the mesh.
         * @param indices The indices of the mesh.
         * @param size The size of the mesh.
         * @param spec The material specifications of the mesh.
         */
        Mesh(std::vector<float>& vertices, std::vector<unsigned int>& indices, const size_t size,
             MaterialSpec spec = DEFAULT)
            : m_vertex_array(std::make_shared<VertexArray>(Convert(vertices, size), std::move(indices), size)),
              m_material(std::make_shared<Material>(std::move(spec)))
        {
        }

        /**
         * @brief Gets the ID of the mesh.
         *
         * @return The ID of the mesh.
         */
        [[nodiscard]] unsigned int GetId() const
        {
            return this->m_vertex_array->GetId();
        }

        /**
         * @brief Sets the material specifications of the mesh.
         *
         * @param spec The material specifications.
         */
        void SetMaterial(MaterialSpec& spec)
        {
            if (!this->m_material)
                this->m_material = std::make_shared<Material>(std::move(spec));
        }

        /**
         * @brief Sets the material specifications of the mesh.
         *
         * @param spec The material specifications.
         */
        void SetMaterial(MaterialSpec&& spec)
        {
            if (!this->m_material)
                this->m_material = std::make_shared<Material>(std::move(spec));
        }

        /**
         * @brief Gets the material associated with the mesh.
         *
         * @return The material.
         */
        [[nodiscard]] const std::shared_ptr<Material>& GetMaterial() const
        {
            return this->m_material;
        }

        /**
         * @brief Gets the constant reference to the vertices of the mesh.
         *
         * @return The constant reference to the vertices.
         */
        [[nodiscard]] const std::vector<Vertex>& c_vertices() const
        {
            return this->m_vertex_array->GetConstVertices();
        }

        /**
         * @brief Gets the reference to the vertices of the mesh.
         *
         * @return The reference to the vertices.
         */
        [[nodiscard]] std::vector<Vertex>& vertices() const
        {
            return this->m_vertex_array->GetVertices();
        }

        /**
         * @brief Gets the constant reference to the indices of the mesh.
         *
         * @return The constant reference to the indices.
         */
        [[nodiscard]] const std::vector<unsigned int>& indices() const
        {
            return this->m_vertex_array->GetIndices();
        }

        /**
         * @brief Gets the number of vertices in the mesh.
         *
         * @return The number of vertices.
         */
        [[nodiscard]] int nVertices() const
        {
            return this->m_vertex_array->GetVertexCount();
        }

        /**
         * @brief Gets the number of indices in the mesh.
         *
         * @return The number of indices.
         */
        [[nodiscard]] int nIndices() const
        {
            return this->m_vertex_array->GetIndexCount();
        }

        /**
         * @brief Sets the draw mode of the mesh.
         *
         * @param mode The draw mode (DEFAULT, INSTANCED, STRIPS, PATCHES).
         */
        void SetDrawMode(const DRAW_MODE mode) const
        {
            this->m_vertex_array->set_mode(mode);
        }

        /**
         * @brief Sets the draw mode information of the mesh.
         *
         * @param info The draw mode information.
         */
        void SetDrawModeInfo(const int info) const
        {
            this->m_vertex_array->set_mode_data(info);
        }

        /**
         * @brief Sets the draw type of the mesh.
         *
         * @param type The draw type (ARRAYS, ELEMENTS).
         */
        void SetDrawType(const DRAW_TYPE type) const
        {
            this->m_vertex_array->set_type(type);
        }

        /**
         * @brief Sets the number of triangles per strip of the mesh.
         *
         * @param info The number of triangles per strip.
         */
        void SetNumTrisPerStrip(const int info) const
        {
            this->m_vertex_array->set_num_tris_per_strip(info);
        }

        /**
         * @brief Draws the mesh.
         */
        void Draw() const
        {
            this->m_vertex_array->Draw();
        }

      private:
        std::shared_ptr<VertexArray> m_vertex_array; /**< The vertex array of the mesh. */
        std::shared_ptr<Material> m_material;        /**< The material of the mesh. */
    };
} // namespace Vakol::Model::Assets