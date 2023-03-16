#include "MeshRenderer.hpp"

#include <Controller/Logger.hpp>
#include <Controller/ModelLoader.hpp>

#include <glad/glad.h>

using Vakol::Model::GLShader;
using Vakol::Model::GLTexture;

using Vakol::Controller::ModelLoader;

namespace Vakol::Controller
{
    std::vector<GLMaterial> MeshRenderer::materials;

    GLMaterial MeshRenderer::Get(const unsigned int index) { return materials.at(index); }

    void MeshRenderer::LoadMaterials()
    {
        for (const auto& mesh : ModelLoader::meshes)
        {   
            materials.push_back(GLMaterial(GLShader("assets/unused/custom.vert", "assets/unused/custom.frag"), mesh.material));
        }
    }

    void MeshRenderer::Draw(const unsigned int type)
    {
        if (type == GL_SHADER)
        {
            for (const auto& material : materials)
                material.Bind(type);
        }
        else
        {
            for (const auto& material : materials)
                material.Bind(type);
        }
    }
}