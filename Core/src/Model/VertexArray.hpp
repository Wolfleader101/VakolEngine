#pragma once 
#include <vector>

namespace Vakol::Model
{
    class VertexArray {
       public:
        VertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
            : vertices(vertices), indices(indices) {}
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        const virtual unsigned int GetID() const = 0;
        const virtual unsigned int GetVertices() const = 0;
        const virtual unsigned int GetIndices() const = 0;

       protected:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    };
}
