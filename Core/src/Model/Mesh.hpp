#pragma once

#include <vector>
#include <memory>

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"

namespace Vakol::Model 
{
    struct Vertex 
    {
        Math::Vec3 position;
        Math::Vec3 normal;
        Math::Vec2 uv;
    };

    struct Mesh 
    {
        Mesh() = default;
        ~Mesh() { this->Clear(); }

        void Clear() 
        {
            vertices.clear();
            indices.clear();
        }

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    struct VertexArrayBuffer
    {
        virtual ~VertexArrayBuffer() = default;

        virtual void Bind() = 0;

        virtual void Unbind() = 0;

        static std::shared_ptr<VertexArrayBuffer> Create(const Mesh& mesh);
    };

    struct VertexBuffer
    {
        virtual ~VertexBuffer() = default;

        virtual void Bind() = 0;

        [[maybe_unused]] virtual void Unbind() = 0;

        static std::shared_ptr<VertexBuffer> Create(const std::vector<Vertex>& vertices);
    };

    struct IndexBuffer
    {
        virtual ~IndexBuffer() = default;

        virtual void Bind() = 0;

        virtual void Unbind() = 0;

        static std::shared_ptr<IndexBuffer> Create(const std::vector<unsigned int>& indices);
    };
}