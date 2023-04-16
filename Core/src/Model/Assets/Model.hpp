#pragma once

#include <vector>
#include <string>

#include "Mesh.hpp"

namespace Vakol::Model::Assets {

    class Model 
    {
    public:
        Model() = default;
        Model(const std::vector<Mesh>& meshes) : m_meshes(meshes){};

        const std::vector<Mesh> meshes() const { return this->m_meshes; }
        const int mesh_count() const { return static_cast<int>(this->m_meshes.size()); }

    private:
        std::vector<Mesh> m_meshes;
    };

}  // namespace Vakol::Model::Assets