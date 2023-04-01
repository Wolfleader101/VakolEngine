#pragma once

#include <Model/GL/GLMesh.hpp>
#include <string>

using Vakol::Model::GLMesh;

namespace Vakol::Controller
{
    class MeshFilter
    {
    public:
        MeshFilter() = default;
        ~MeshFilter() {};

        static void LoadMesh(const std::string& path);
        static void Draw(const unsigned int mode);

        static std::vector<GLMesh> meshes;
    };
}
