#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Mesh.hpp"
#include "Shader.hpp"

namespace Vakol::Model::Assets 
{
    class Model 
    {
    public:
        Model() = default;

        Model(Mesh& mesh) { m_meshes.push_back(std::move(mesh)); }
        Model(Mesh&& mesh) { m_meshes.push_back(std::move(mesh)); }

        Model(std::vector<Mesh>&& meshes) : m_meshes(std::move(meshes)) {};
        Model(std::vector<Mesh>& meshes) : m_meshes(std::move(meshes)) {};

        void SetShader(const std::string& path) 
        { 
            this->m_shader = std::make_shared<Shader>(path);
            this->m_shader->Bind(); 
        }
        
        std::shared_ptr<Shader> GetShader() { return m_shader; }
        
        Mesh GetMesh(const int index = 0) const { return m_meshes.at(index); }


        const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
        
        const int GetMeshCount() const { return static_cast<int>(m_meshes.size()); }

    private:
        std::vector<Mesh> m_meshes;
        std::shared_ptr<Shader> m_shader = nullptr;
    };

}