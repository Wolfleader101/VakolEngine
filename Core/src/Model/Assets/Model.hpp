#pragma once

#include <memory>

#include "Mesh.hpp"
#include "Model/Shader.hpp"

namespace Vakol::Model::Assets
{
    class Model
	{
    public:
        Model() = default;

        Model(Mesh& mesh) { m_meshes.push_back(std::move(mesh)); }
        Model(Mesh&& mesh) { m_meshes.push_back(std::move(mesh)); }

        Model(std::vector<Mesh>& meshes) : m_meshes(std::move(meshes)) {}
        Model(std::vector<Mesh>&& meshes) : m_meshes(std::move(meshes)) {}

        void set_shader(const std::shared_ptr<Shader>& shader)
    	{
            this->m_shader = shader;
            this->m_shader->Bind();
        }

		[[nodiscard]] bool cullBackface() const { return m_cullBackface; }

    	void SetCullBackface(const bool cull) { m_cullBackface = cull; }

        std::shared_ptr<Shader>& shader() { return m_shader; }
        [[nodiscard]] const std::shared_ptr<Shader>& c_shader() const { return m_shader; }

        [[nodiscard]] const Mesh& mesh(const int index = 0) const { return m_meshes.at(index); }

        [[nodiscard]] const std::vector<Mesh>& c_meshes() const { return m_meshes; }
        [[nodiscard]] std::vector<Mesh>& meshes() { return m_meshes; }

        [[nodiscard]] int nMeshes() const { return static_cast<int>(m_meshes.size()); }

    private:
        std::vector<Mesh> m_meshes;
        std::shared_ptr<Shader> m_shader = nullptr;

        bool m_animated = false;
        bool m_cullBackface = true;
    };

}