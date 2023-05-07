#pragma once

#include <memory>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Animation.hpp"

namespace Vakol::Model::Assets 
{
    class Model 
    {
    public:
        Model() = default;

        Model(Mesh& mesh) { m_meshes.push_back(std::move(mesh)); }
        Model(Mesh&& mesh) { m_meshes.push_back(std::move(mesh)); }

        Model(std::vector<Mesh>&& meshes) : m_meshes(std::move(meshes)) {}
        Model(std::vector<Mesh>& meshes) : m_meshes(std::move(meshes)) {}

        Model(std::vector<Mesh>&& meshes, Animation&& animation) : m_meshes(std::move(meshes)), m_animation(std::move(animation)) {}
        Model(std::vector<Mesh>& meshes, Animation& animation) : m_meshes(std::move(meshes)), m_animation(std::move(animation)) {}

        void SetShader(const std::string& path) 
        { 
            this->m_shader = std::make_shared<Shader>(path);
            this->m_shader->Bind(); 
        }
        
        std::shared_ptr<Shader> GetShader() { return m_shader; }
        
        [[nodiscard]] Mesh nMesh(const int index = 0) const { return m_meshes.at(index); }

        [[nodiscard]] Animation& animation() { return m_animation;  }

        [[nodiscard]] const std::vector<Mesh>& meshes() const { return m_meshes; }
        [[nodiscard]] std::vector<Mesh>& meshes() { return m_meshes; }

    	[[nodiscard]] int nMeshes() const { return static_cast<int>(m_meshes.size()); }

    private:
        std::vector<Mesh> m_meshes;
        Animation m_animation {};
        std::shared_ptr<Shader> m_shader = nullptr;
    };

}