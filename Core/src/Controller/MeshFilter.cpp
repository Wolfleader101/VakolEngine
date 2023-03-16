#include "MeshFilter.hpp"

#include "ModelLoader.hpp"

using Vakol::Model::DRAW_MODE_INDEXED;
using Vakol::Model::DRAW_MODE_NON_INDEXED;

namespace Vakol::Controller
{
    std::vector<GLMesh> MeshFilter::meshes;

    void MeshFilter::LoadMesh(const std::string& path)
    {
        ModelLoader::LoadModel(path);

        for (const auto& mesh : ModelLoader::meshes)
        {
            meshes.push_back(GLMesh(mesh.vertices, mesh.indices));
        }
    }

    void MeshFilter::Draw(const unsigned int mode)
    {
        if (mode == DRAW_MODE_INDEXED)
            for (const auto& mesh : meshes)
                mesh.Draw(mode);
        else if (mode == DRAW_MODE_NON_INDEXED)
            for (const auto& mesh : meshes)
                mesh.Draw(mode);
    }
}