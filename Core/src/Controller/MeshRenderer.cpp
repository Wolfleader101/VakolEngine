#include "MeshRenderer.hpp"

#include "ModelLoader.hpp"

#include <Controller/Logger.hpp>

#include <glad/glad.h>

using Vakol::Model::GLShader;
using Vakol::Model::GLTexture;

namespace Vakol::Controller
{
    void MeshRenderer::SetMesh(const std::string& path)
    {
        ModelLoader::LoadModel(path);

        for (const auto& mesh : ModelLoader::meshes)
        {
            this->meshes.push_back(GLMesh(mesh.vertices, mesh.indices));

            //VK_TRACE("{0}", mesh.textures.size());

            this->materials.push_back(GLMaterial(GLShader("assets/unused/custom.vert", "assets/unused/custom.frag"), mesh.textures));
        }
    }

    GLMaterial MeshRenderer::Get(const unsigned int index) const
    {
        return materials.at(index);
    }

    void MeshRenderer::Draw(const unsigned int type) const
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

            for (const auto& mesh : meshes)
                mesh.Draw();
        }
    }
}