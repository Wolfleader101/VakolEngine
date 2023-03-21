#include "MeshFilter.hpp"

#include "ModelLoader.hpp"

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
        for (const auto& mesh : meshes)
            mesh.Draw(mode);
    }
}