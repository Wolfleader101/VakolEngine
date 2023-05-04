#include "VertexArray.hpp"

#include <glad/glad.h>
#include <Controller/Logger.hpp>

#include <iostream>

#define VERBOSE_DEBUG 1

namespace Vakol::Model
{
    const std::vector<float> Convert(const std::vector<Vertex>& arr, const int size)
    {
        std::vector<float> output;

        const auto arr_size = static_cast<int>(arr.size());
        const auto elements = static_cast<int>(size / sizeof(float));

    #if VERBOSE_DEBUG
        VK_TRACE("Converting vector of floats to vector of Vertex.");

        std::cout << std::endl;

        VK_TRACE("Vector info:");
        std::cout << std::endl;
        VK_TRACE("Vector Size = {0}", arr_size);
        VK_TRACE("Vertex Size = {0}", size);
        VK_TRACE("Vertex Elements = {0} (number of floats)", elements);

        std::cout << std::endl;
    #endif

        VK_ASSERT(arr.size() > 0, "\n\nNo point converting an empty vector");
        VK_ASSERT(elements == 3 || elements == 5 || elements == 8 || elements == 14, "\n\nUnknown Vertex size.");

        output.reserve(arr.size() * elements);

        for (int i = 0; i < arr_size; i++) // this is really hacky, but it works, aaaaand i'm just lazy
        {
            auto& vertex = arr.at(i);
            
            if (elements == 3)
            {
                output.push_back(vertex.position.x); // Position 0
                output.push_back(vertex.position.y);
                output.push_back(vertex.position.z);
                VK_TRACE("positions");
            }

            if (elements == 5) // why do we check for this twice? Sometimes people only want positions and texture coordinates (i.e. skyboxes) save time and data
            {
                output.push_back(vertex.position.x); // Position 0
                output.push_back(vertex.position.y);
                output.push_back(vertex.position.z);
                VK_TRACE("positions");

                output.push_back(vertex.uv.x); // Position 1
                output.push_back(vertex.uv.y);
                VK_TRACE("uvs");
            }

            if (elements == 8)
            {
                output.push_back(vertex.position.x); // Position 0
                output.push_back(vertex.position.y);
                output.push_back(vertex.position.z);
                VK_TRACE("positions");

                output.push_back(vertex.normal.x); // Position 1 (if size is large enough)
                output.push_back(vertex.normal.y);
                output.push_back(vertex.normal.z);
                VK_TRACE("normals");

                output.push_back(vertex.uv.x); // Position 2 (if size is large enough)
                output.push_back(vertex.uv.y);
                VK_TRACE("uvs");
            }

            if (elements == 14) // you need uvs and normals in order to get the resultant tangent and bitangent (THEY MUST STICK TOGETHER)
            {
                output.push_back(vertex.position.x); // Position 0
                output.push_back(vertex.position.y);
                output.push_back(vertex.position.z);

                output.push_back(vertex.normal.x); // Position 1 (if size is large enough)
                output.push_back(vertex.normal.y);
                output.push_back(vertex.normal.z);

                output.push_back(vertex.uv.x); // Position 2 (if size is large enough)
                output.push_back(vertex.uv.y);

                output.push_back(vertex.tangent.x); // Position 3
                output.push_back(vertex.tangent.y);
                output.push_back(vertex.tangent.z);
            
                output.push_back(vertex.bitangent.x); // Position 4
                output.push_back(vertex.bitangent.y);
                output.push_back(vertex.bitangent.z);
            }
        }

        arr.~vector();

        return output;
    }

    VertexArray::VertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const int size) : vertices(std::move(vertices)), indices(std::move(indices))
    {
        int total_elements = size / sizeof(float);
        auto used_elements = 0;

        this->n_vertices = static_cast<int>(this->vertices.size());
        this->n_indices = static_cast<int>(this->indices.size());

        VK_ASSERT(n_vertices > 0, "\n\nCannot create empty vertex array.");

    #if VERBOSE_DEBUG
        std::cout << std::endl;

        VK_TRACE("Vector info:");

        std::cout << std::endl;

        VK_TRACE("Vector Size = {0}", n_vertices);
        VK_TRACE("Vertex Size = {0}", size);
        VK_TRACE("Vertex Elements = {0} (number of floats)", total_elements);

        std::cout << std::endl;
    #endif

        this->GenArray(1, &this->ID);
        glGenBuffers(1, &this->VBO);
        if (n_indices > 0) glGenBuffers(1, &this->EBO); // EBO is optional

        VK_ASSERT(this->VBO != 0 && (n_indices > 0) ? this->EBO != 0 : 1, "Failed to generate one or more buffers!");

        this->Bind();

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);

        if (n_indices > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indices * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);
        }

        if (total_elements >= 3)
        {
            this->SetVertexAttribData(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0); // Positions
            used_elements += 3;
            total_elements -= 3;
        }

        if (total_elements >= 2)
        {
            if (total_elements >= 3)
            {
                this->SetVertexAttribData(1, 3, GL_FLOAT, GL_FALSE, size, (void*)(used_elements * sizeof(float))); // Normals
                total_elements -= 3;
                used_elements += 3;
            }
            else
            {
                this->SetVertexAttribData(1, 2, GL_FLOAT, GL_FALSE, size, (void*)(used_elements * sizeof(float))); // UVs
                total_elements -= 2;
                used_elements += 2;
            }
        }

        if (total_elements >= 2)
        {
            this->SetVertexAttribData(2, 2, GL_FLOAT, GL_FALSE, size, (void*)(used_elements * sizeof(float))); // UVs (with normals)
            total_elements -= 2;
            used_elements += 2;
        }

        if (total_elements >= 3)
        {
            this->SetVertexAttribData(3, 3, GL_FLOAT, GL_FALSE, size, (void*)(used_elements * sizeof(float))); // Tangents
            total_elements -= 3;
            used_elements += 3;
        }

        if (total_elements >= 3)
        {
            this->SetVertexAttribData(4, 3, GL_FLOAT, GL_FALSE, size, (void*)(used_elements * sizeof(float)));
            total_elements -= 3;
            used_elements += 3;
        }

        VK_ASSERT(total_elements == 0, "\n\nReserved Vertex data was not fully allocated.");

        glPatchParameteri(GL_PATCH_VERTICES, NUM_PATCH_PTS);

        this->Unbind();
        glBindBuffer(GL_ARRAY_BUFFER, 0); // turns out this is perfectly legal, don't unbind EBO though!
    }

    VertexArray::~VertexArray() { glDeleteVertexArrays(1, &this->ID); }

    void VertexArray::Draw() const
    {
        this->Bind();

        if (this->info.draw_mode == DRAW_MODE::DEFAULT)
        {
            if (this->info.draw_type == DRAW_TYPE::ARRAYS)
                glDrawArrays(GL_TRIANGLES, 0, this->n_vertices);
            else
                glDrawElements(GL_TRIANGLES, this->n_indices, GL_UNSIGNED_INT, 0);
        }

        if (this->info.draw_mode == DRAW_MODE::INSTANCED)
        {
            if (this->info.draw_type == DRAW_TYPE::ARRAYS)
                glDrawArraysInstanced(GL_TRIANGLES, 0, this->n_vertices, this->info.INSTANCE_AMOUNT);
            else
                glDrawElementsInstanced(GL_TRIANGLES, this->n_indices, GL_UNSIGNED_INT, 0, this->info.INSTANCE_AMOUNT);
        }

        if (this->info.draw_mode == DRAW_MODE::STRIPS)
        {
            if (this->info.draw_type == DRAW_TYPE::ARRAYS)
            {
                // idk lol
            }
            else
            {
                for (unsigned int strip = 0; strip < this->info.NUM_STRIPS; ++strip)
                    glDrawElements(GL_TRIANGLE_STRIP, this->info.NUM_TRIS_PER_STRIP + 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * (this->info.NUM_TRIS_PER_STRIP + 2) * strip));
            }
        }

        if (this->info.draw_mode == DRAW_MODE::PATCHES)
        {
            if (this->info.draw_type == DRAW_TYPE::ARRAYS)
                glDrawArrays(GL_PATCHES, 0, this->info.NUM_PATCHES * this->info.NUM_VERTS_PER_PATCH);
            else
            {
                // idk lol
            }
        }

        this->Unbind();
    }

    void VertexArray::GenArray(const unsigned int n, unsigned int* array) { glGenVertexArrays(static_cast<GLsizei>(n), static_cast<GLuint*>(array)); }

    void VertexArray::SetVertexAttribData(const int index, const int n, const unsigned int type, const bool normalized, const int stride, const void* data)
    {
        glEnableVertexAttribArray(index); // enable the vertex attribute at the specific index/location first
        glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(n), static_cast<GLenum>(type), normalized, static_cast<GLsizei>(stride), data);
    }

    void VertexArray::Bind() const { glBindVertexArray(this->ID); }

    void VertexArray::Unbind() const { glBindVertexArray(0); }
}