#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Mesh.hpp"
#include <Model/gl/GLShader.hpp>

namespace Vakol::Model::Assets 
{
    class Model 
    {
    public:
        Model() = default;
        Model(const std::vector<Mesh>& meshes) : m_meshes(meshes) {};

        void SetShader(const std::string& path) { this->m_shader = std::make_shared<GLShader>(path); }
        std::shared_ptr<Shader> shader() { return m_shader; }

        const std::vector<Mesh> meshes() const { return m_meshes; }
        const int mesh_count() const { return static_cast<int>(m_meshes.size()); }

    private:
        std::vector<Mesh> m_meshes;
        std::shared_ptr<Shader> m_shader = nullptr;
    };

}  // namespace Vakol::Model::Assets