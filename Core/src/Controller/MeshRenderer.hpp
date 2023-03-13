#pragma once

#include <Model/GL/GLMesh.hpp>
#include <Model/GL/GLTexture.hpp>
#include <Model/GL/GLMaterial.hpp>

using Vakol::Model::GLMesh;
using Vakol::Model::GLMaterial;

namespace Vakol::Controller
{
    class MeshRenderer
    {
    public:
        MeshRenderer() = default;
        ~MeshRenderer() {};
 
        void SetMesh(const std::string& path);

        void Draw(const unsigned int type) const;
        GLMaterial Get(const unsigned int index) const;

        std::vector<GLMesh> meshes;
        std::vector<GLMaterial> materials;
    };
}
