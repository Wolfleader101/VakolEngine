#pragma once

#include <memory>

#include "Mesh.hpp"
#include "Model/Shader.hpp"

namespace Vakol::Model::Assets
{
    /**
     * @brief Class representing a model.
     */
    class Model
    {
      public:
        Model() = default;

        /**
         * @brief Constructs a Model object with the given mesh.
         *
         * @param mesh The mesh of the model.
         */
        Model(Mesh &mesh)
        {
            m_meshes.push_back(std::move(mesh));
        }

        /**
         * @brief Constructs a Model object with the given mesh.
         *
         * @param mesh The mesh of the model.
         */
        Model(Mesh &&mesh)
        {
            m_meshes.push_back(std::move(mesh));
        }

        /**
         * @brief Constructs a Model object with the given vector of meshes.
         *
         * @param meshes The vector of meshes.
         */
        Model(std::vector<Mesh> &meshes) : m_meshes(std::move(meshes))
        {
        }

        /**
         * @brief Constructs a Model object with the given vector of meshes.
         *
         * @param meshes The vector of meshes.
         */
        Model(std::vector<Mesh> &&meshes) : m_meshes(std::move(meshes))
        {
        }

        /**
         * @brief Sets the shader associated with the model.
         *
         * @param shader The shader.
         */
        void set_shader(const std::shared_ptr<Shader> &shader)
        {
            this->m_shader = shader;
            this->m_shader->Bind();
        }

        /**
         * @brief Checks if backface culling is enabled.
         *
         * @return True if backface culling is enabled, false otherwise.
         */
        [[nodiscard]] bool cullBackface() const
        {
            return m_cullBackface;
        }

        /**
         * @brief Sets whether backface culling should be enabled or disabled.
         *
         * @param cull True to enable backface culling, false to disable it.
         */
        void SetCullBackface(const bool cull)
        {
            m_cullBackface = cull;
        }

        /**
         * @brief Gets the shader associated with the model.
         *
         * @return The shader.
         */
        std::shared_ptr<Shader> &shader()
        {
            return m_shader;
        }

        /**
         * @brief Gets the constant reference to the shader associated with the model.
         *
         * @return The constant reference to the shader.
         */
        [[nodiscard]] const std::shared_ptr<Shader> &c_shader() const
        {
            return m_shader;
        }

        /**
         * @brief Gets the mesh at the specified index.
         *
         * @param index The index of the mesh.
         * @return The mesh.
         */
        [[nodiscard]] const Mesh &mesh(const int index = 0) const
        {
            return m_meshes.at(index);
        }

        /**
         * @brief Gets the constant reference to the vector of meshes associated with the model.
         *
         * @return The constant reference to the vector of meshes.
         */
        [[nodiscard]] const std::vector<Mesh> &c_meshes() const
        {
            return m_meshes;
        }

        /**
         * @brief Gets the reference to the vector of meshes associated with the model.
         *
         * @return The reference to the vector of meshes.
         */
        [[nodiscard]] std::vector<Mesh> &meshes()
        {
            return m_meshes;
        }

        /**
         * @brief Gets the number of meshes in the model.
         *
         * @return The number of meshes.
         */
        [[nodiscard]] int nMeshes() const
        {
            return static_cast<int>(m_meshes.size());
        }

      private:
        std::vector<Mesh> m_meshes;                 /**< The vector of meshes associated with the model. */
        std::shared_ptr<Shader> m_shader = nullptr; /**< The shader associated with the model. */
        bool m_animated = false;                    /**< Flag indicating if the model is animated. */
        bool m_cullBackface = true;                 /**< Flag indicating if backface culling is enabled. */
    };

} // namespace Vakol::Model::Assets