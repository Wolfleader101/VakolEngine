#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Buffer.hpp"

const unsigned char ARRAYS = 0x0;
const unsigned char ELEMENTS = 0x1;
const unsigned char TRIANGLE_STRIP = 0x2;
const unsigned char QUAD_STRIP = 0x3;
const unsigned char INSTANCED_ARRAYS = 0x4;
const unsigned char INSTANCED_ELEMENTS = 0x5;

namespace Vakol::Model
{
    constexpr int MAX_BONE_INFLUENCE = 4;

    struct Vertex 
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        //glm::vec3 tangent;
        //glm::vec3 bitangent;
    };

    const std::vector<float> Convert(const std::vector<Vertex>& arr, const int size);

    class VertexArray
    {
    public:
        VertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const int size);
        ~VertexArray();

        void Draw() const;

        void GenArray(const unsigned int n, unsigned int* array);
        void EnableVertexAttribArray(const unsigned int location);

        /// @brief Specify the data of a vertex attribute.
        /// @param index The location at which this vertex attribute occurs. 
        /// Example: Position (aPos) = 0, Normal (aNormal) = 1, UV (aTexCoords) = 2
        /// @param n number of elements for which the attribute contains. 
        /// Example: float = 1, glm::vec2 = 2 glm::vec3 = 3, glm::vec4 = 4 (MAX SIZE = 4)
        /// @param type the type of data to be used
        /// Example: GL_FLOAT
        /// @param normalized IF TRUE - have the data be normalized between ([-1, 1] (signed)) and ([0, 1] (unsigned)) 
        /// @param stride byte offset between each consecutive vertex *just keep this at the size of a Vertex* (Our data is not tightly packed).
        /// @param data  the byte offset from the starting vertex attribute
        /// Example 1: Position (aPos) = (void*)0 -> *Position does not need any offset since it starts first*
        /// Example 2: Normal (aNormal) = (void*)offsetof(Vertex, normal) *if using Vertex struct* OR (void*)(3 * sizeof(float))
        void SetVertexAttribData(const int index, const int n, const unsigned int type, const bool normalized, const int stride, const void* data);

        void Bind() const;
        void Unbind() const;
        
        inline const unsigned int GetID() const { return this->ID; }

        inline const int GetVertexCount() const { return n_vertices; }
        inline const int GetIndexCount() const { return n_indices; }
    private:
        unsigned int ID = 0;

        Buffer VBO;
        Buffer EBO;

    private:
        int n_vertices = 0;
        int n_indices = 0;

    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    };
}