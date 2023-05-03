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
        Model(const Mesh& mesh) { m_meshes.push_back(mesh); }
        Model(const std::vector<Mesh>& meshes) : m_meshes(meshes) {};

        void SetShader(const std::string& path) 
        { 
            this->m_shader = std::make_shared<Shader>(path);
            this->m_shader->Bind(); 
        }
        
        std::shared_ptr<Shader> GetShader() { return m_shader; }
        
        const Mesh GetMesh() const { return m_meshes.at(0); }
        const std::vector<Mesh> GetMeshes() const { return m_meshes; }
        
        const int GetMeshCount() const { return static_cast<int>(m_meshes.size()); }

    private:
        std::vector<Mesh> m_meshes;
        std::shared_ptr<Shader> m_shader = nullptr;
    };

}