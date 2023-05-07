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

        explicit Model(Mesh&& mesh) { m_meshes.push_back(std::move(mesh)); }

    	Model(std::vector<Mesh>&& meshes) : m_meshes(std::move(meshes)) {}

        Model(std::vector<Mesh>&& meshes, std::vector<Animation>&& animations) : m_meshes(std::move(meshes)), m_animations(std::move(animations)) {}

        void SetShader(const std::string& path) 
        { 
            this->m_shader = std::make_shared<Shader>(path);
            this->m_shader->Bind(); 
        }
        
        std::shared_ptr<Shader> GetShader() { return m_shader; }
        
        [[nodiscard]] Mesh nMesh(const int index = 0) const { return m_meshes.at(index); }

        [[nodiscard]] const std::vector<Animation>& animations() const { return m_animations;  }
        [[nodiscard]] std::vector<Animation>& animations() { return m_animations; }

        [[nodiscard]] const std::vector<Mesh>& meshes() const { return m_meshes; }
        [[nodiscard]] std::vector<Mesh>& meshes() { return m_meshes; }

    	[[nodiscard]] int nMeshes() const { return static_cast<int>(m_meshes.size()); }

    private:
        std::vector<Mesh> m_meshes;
        std::vector<Animation> m_animations;
        std::shared_ptr<Shader> m_shader = nullptr;
    };

}