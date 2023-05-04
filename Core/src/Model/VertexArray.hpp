#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "Buffer.hpp"

#define USE_TRIANGLE_PATCHES 0

class Mesh;

namespace Vakol::Model
{
    constexpr int MAX_BONE_INFLUENCE = 4;

#if USE_TRIANGLE_PATCHES
    constexpr int NUM_PATCH_PTS = 3;
#else
    constexpr int NUM_PATCH_PTS = 4;
#endif

    struct Vertex 
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    enum class DRAW_MODE
    {
        DEFAULT, INSTANCED, STRIPS, PATCHES
    };

    enum class DRAW_TYPE
    {
        ARRAYS, ELEMENTS
    };

    const unsigned int TRIANGLES = 4;
    const unsigned int QUADS = 7;
    const unsigned int TRIANGLE_STRIP = 5;
    const unsigned int QUAD_STRIP = 8;
    const unsigned int PATCHES = 15;

    struct DrawInfo
    {
        DRAW_MODE draw_mode = DRAW_MODE::DEFAULT;
        unsigned int draw_shape = TRIANGLES;
        DRAW_TYPE draw_type = DRAW_TYPE::ELEMENTS;

    // instancing info
        unsigned int INSTANCE_AMOUNT = 0;

    // strip info
        unsigned int NUM_STRIPS = 0;

    // triangle strip info
        unsigned int NUM_TRIS_PER_STRIP = 0;

    // quad strip info
        unsigned int NUM_QUADS_PER_STRIP = 0;

    // patch info
        unsigned int NUM_PATCHES = 0;
        const int NUM_VERTS_PER_PATCH = NUM_PATCH_PTS;
    };

    const std::vector<float> Convert(const std::vector<Vertex>& arr, const int size);

    class VertexArray
    {
    public:
        VertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const int size);
        ~VertexArray();

        void Draw() const;

        void GenArray(const unsigned int n, unsigned int* array);

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

        inline const int GetVertexCount() const { return this->n_vertices; }
        inline const int GetIndexCount() const { return this->n_indices; }

        inline const std::vector<float>& GetVertices() const { return this->vertices; }

        inline void set_mode(const DRAW_MODE mode) { this->info.draw_mode = mode; }
        inline void set_shape(const unsigned int type) { this->info.draw_shape = type; }
        inline void set_type(const DRAW_TYPE type) { this->info.draw_type = type; }

        inline void set_mode_data(const unsigned int data) 
        {
            if (this->info.draw_mode == DRAW_MODE::INSTANCED)
                this->info.INSTANCE_AMOUNT = data;

            if (this->info.draw_mode == DRAW_MODE::STRIPS)
                this->info.NUM_STRIPS = data;

            if (this->info.draw_mode == DRAW_MODE::PATCHES)
                this->info.NUM_PATCHES = data;
        }

        inline void set_type_data(const unsigned int data)
        {
            if (this->info.draw_mode == DRAW_MODE::STRIPS && this->info.draw_shape == TRIANGLE_STRIP)
                this->info.NUM_TRIS_PER_STRIP = data;

            if (this->info.draw_mode == DRAW_MODE::STRIPS && this->info.draw_shape == QUAD_STRIP)
                this->info.NUM_QUADS_PER_STRIP = data;
        }

    private:
        unsigned int ID = 0;

        Buffer VBO;
        Buffer EBO;

    private:
        int n_vertices = 0;
        int n_indices = 0;

        DrawInfo info;
    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    };
}