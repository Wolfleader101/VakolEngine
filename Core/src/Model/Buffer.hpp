#pragma once

#include <memory>

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"

namespace Vakol::Model 
{
    struct Vertex 
    {
        Math::Vec3 position;
        //Math::Vec3 normal;
        //Math::Vec2 uv;
    };

    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual const unsigned int GetID() const = 0;
    };

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;
    };
}