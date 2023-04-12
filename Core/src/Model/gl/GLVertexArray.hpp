#pragma once 

#include <Model/VertexArray.hpp>

namespace Vakol::Model
{
    class GLVertexArray : public VertexArray {
       public:
        GLVertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~GLVertexArray();

        void GenArray(const unsigned int n, unsigned int* array);
        void GenBuffer(const unsigned int n, unsigned int* buffer);

        void BindBuffer(const unsigned int type, const unsigned int buffer);

        void Bind() const override;
        void Unbind() const override;

        void Draw() const override;

        const unsigned int GetID() const override;
        const unsigned int GetVertices() const override;
        const unsigned int GetIndices() const override;

    private:
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;
    };
}