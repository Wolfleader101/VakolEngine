#pragma once

#include <vector>

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"

namespace Vakol::Model {
    struct Vertex {
        Math::Vec3 position;
        // ADD THIS LATER//Vector3 normal;
        Math::Vec2 uv;
    };

    struct Mesh {
        Mesh() = default;
        ~Mesh() { this->Clear(); }

        void Clear() {
            vertices.clear();
            indices.clear();
        }

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

       private:
        unsigned int id;
    };
}  // namespace Vakol::Model
