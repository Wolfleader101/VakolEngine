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

    GLMaterial MeshRenderer::Get(const unsigned int index) 
    { 
        try 
        {
            return materials.at(index); 
        }
        catch (std::exception e)
        {
            VK_CRITICAL("ERROR::CORE::MESH_RENDERER: {0}", e.what());
            quick_exit(EXIT_FAILURE);
        }
    }

    void MeshRenderer::LoadMaterials()
    {
        for (const auto& mesh : ModelLoader::meshes)
        {   
            materials.push_back(GLMaterial(GLShader("assets/unused/custom.vert", "assets/unused/custom.frag"), mesh.materialInfo));
        }
    }

    void MeshRenderer::Draw(const unsigned int type)
    {
        for (const auto& material : materials)
            material.Bind(type);
    }
}