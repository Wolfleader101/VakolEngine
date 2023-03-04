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

    class Mesh 
    {
    public:
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

    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        static std::shared_ptr<VertexArray> Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        static std::shared_ptr<VertexBuffer> Create(const std::vector<Vertex>& vertices);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        static std::shared_ptr<IndexBuffer> Create(const std::vector<unsigned int>& indices);
    };
}