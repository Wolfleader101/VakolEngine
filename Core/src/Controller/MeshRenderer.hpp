#pragma once

#include <Model/GL/GLTexture.hpp>
#include <Model/GL/GLMaterial.hpp>


using Vakol::Model::GLMaterial;

namespace Vakol::Controller
{
    class MeshRenderer
    {
    public:
        MeshRenderer() = default;
        ~MeshRenderer() {};

        static void LoadMaterials(); 
        
        static void Draw(const unsigned int type);
        static GLMaterial Get(const unsigned int index);

        static std::vector<GLMaterial> materials;
    };
}
