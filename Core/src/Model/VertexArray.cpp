#include "VertexArray.hpp"

#include <glad/glad.h>
#include <Controller/Logger.hpp>

#include <iostream>

#define VERBOSE_DEBUG 0

namespace Vakol::Model
{
    std::vector<Vertex>& Convert(std::vector<float>& arr)
    {
        const auto arr_size = arr.size();
        constexpr auto elements = sizeof(Vertex) / sizeof(float);

#if VERBOSE_DEBUG
        VK_TRACE("Converting vector of Vertex to vector of floats.");

        std::cout << std::endl;

        VK_TRACE("Vector info:");
        std::cout << std::endl;
        VK_TRACE("Vector Size = {0}", arr_size);
        VK_TRACE("Vertex Size = {0}", size);
        VK_TRACE("Vertex Elements = {0} (number of floats)", elements);

        std::cout << std::endl;
#endif

        VK_ASSERT(!arr.empty(), "\n\nNo point converting an empty vector");
        VK_ASSERT(elements == 3 || elements == 5 || elements == 8 || elements == 14 || elements == 22, "\n\nUnknown Vertex size.");

    	std::vector<Vertex> output;

        output.reserve(arr_size / elements);

        for (auto i = static_cast<size_t>(0); i < arr_size; i += elements)
        {
            Vertex vertex{};

            vertex.position = { arr.at(i), arr.at(i + 1), arr.at(i + 2) };
            vertex.normal = { arr.at(i + 3), arr.at(i + 4), arr.at(i + 5) };
            vertex.uv = { arr.at(6), arr.at(7) };
            vertex.tangent = { arr.at(8), arr.at(9), arr.at(10) };
            vertex.bitangent = { arr.at(11), arr.at(12), arr.at(13) };

            vertex.bone_ids[0] = static_cast<int>(arr.at(14));
            vertex.bone_ids[1] = static_cast<int>(arr.at(15));
            vertex.bone_ids[2] = static_cast<int>(arr.at(16));
            vertex.bone_ids[3] = static_cast<int>(arr.at(17));

            vertex.bone_weights[0] = arr.at(18);
            vertex.bone_weights[1] = arr.at(19);
            vertex.bone_weights[2] = arr.at(20);
            vertex.bone_weights[3] = arr.at(21);

            output.push_back(vertex);
        }

        arr.~vector();

        return output;
    }

	std::vector<float> Convert(std::vector<Vertex>& arr)
    {
        std::vector<float> output;

        const auto arr_size = arr.size();
        constexpr auto elements = sizeof(Vertex) / sizeof(float);

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

        VK_ASSERT(!arr.empty(), "\n\nNo point converting an empty vector");
        VK_ASSERT(elements == 3 || elements == 5 || elements == 8 || elements == 14 || elements == 22, "\n\nUnknown Vertex size.");

        output.reserve(arr.size() * elements);

        for (auto i = static_cast<size_t>(0); i < arr_size; i++) // this is really hacky, but it works, aaaaand i'm just lazy
        {
            const auto& [position, normal, uv, tangent, bitangent, bone_ids, bone_weights] = arr.at(i);

            output.push_back(position.x); // Position 0
            output.push_back(position.y);
            output.push_back(position.z);

            output.push_back(normal.x); // Position 1
            output.push_back(normal.y);
            output.push_back(normal.z);

            output.push_back(uv.x); // Position 2 
            output.push_back(uv.y);

            output.push_back(tangent.x); // Position 3
            output.push_back(tangent.y);
            output.push_back(tangent.z);

            output.push_back(bitangent.x); // Position 4
            output.push_back(bitangent.y);
            output.push_back(bitangent.z);

            output.push_back(static_cast<float>(bone_ids[0])); // Position 5
            output.push_back(static_cast<float>(bone_ids[1]));
            output.push_back(static_cast<float>(bone_ids[2]));
            output.push_back(static_cast<float>(bone_ids[3]));

            output.push_back(bone_weights[0]); // Position 6
            output.push_back(bone_weights[1]);
            output.push_back(bone_weights[2]);
            output.push_back(bone_weights[3]);
        }

        arr.~vector();

        return output;
    }

    VertexArray::VertexArray(std::vector<float>&& vertices, std::vector<unsigned int>&& indices, const int size) : vertices(vertices), indices(indices)
    {
        int total_elements = static_cast<int>(size / sizeof(float));
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
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(n_vertices * sizeof(float)), this->vertices.data(), GL_STATIC_DRAW);

        if (n_indices > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(n_indices * sizeof(unsigned int)), this->indices.data(), GL_STATIC_DRAW);
        }

        if (total_elements >= 3)
        {
            this->SetVertexAttributeData(0, 3, GL_FLOAT, GL_FALSE, size, nullptr); // Positions
            used_elements += 3;
            total_elements -= 3;
        }

        if (total_elements >= 2)
        {
            if (total_elements >= 3)
            {
                this->SetVertexAttributeData(1, 3, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(used_elements * sizeof(float))); // Normals
                total_elements -= 3;
                used_elements += 3;
            }
            else
            {
                this->SetVertexAttributeData(1, 2, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(used_elements * sizeof(float))); // UVs
                total_elements -= 2;
                used_elements += 2;
            }
        }

        if (total_elements >= 2)
        {
            this->SetVertexAttributeData(2, 2, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(used_elements * sizeof(float))); // UVs (with normals)
            total_elements -= 2;
            used_elements += 2;
        }

        if (total_elements >= 3)
        {
            this->SetVertexAttributeData(3, 3, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(used_elements * sizeof(float))); // Tangents
            total_elements -= 3;
            used_elements += 3;
        }

        if (total_elements >= 3)
        {
            this->SetVertexAttributeData(4, 3, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(used_elements * sizeof(float))); // Bitangents
            total_elements -= 3;
        	used_elements += 3;
        }

        if (total_elements >= 4)
        {
            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, size, reinterpret_cast<const void*>(used_elements * sizeof(float))); // Bone IDs
            total_elements -= 4;
            used_elements += 4;
        }

        if (total_elements >= 4)
        {
            this->SetVertexAttributeData(6, 4, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(used_elements * sizeof(float))); // Bone Weights
            total_elements -= 4;
            used_elements += 4;
        }

        VK_ASSERT(total_elements == 0 && used_elements == static_cast<int>(size / sizeof(float)), "\n\nReserved Vertex data was not fully allocated.");

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
                glDrawElements(GL_TRIANGLES, this->n_indices, GL_UNSIGNED_INT, nullptr);
        }

        if (this->info.draw_mode == DRAW_MODE::INSTANCED)
        {
            if (this->info.draw_type == DRAW_TYPE::ARRAYS)
                glDrawArraysInstanced(GL_TRIANGLES, 0, this->n_vertices, this->info.INSTANCE_AMOUNT);
            else
                glDrawElementsInstanced(GL_TRIANGLES, this->n_indices, GL_UNSIGNED_INT, nullptr, this->info.INSTANCE_AMOUNT);
        }

        if (this->info.draw_mode == DRAW_MODE::STRIPS)
        {
            if (this->info.draw_type == DRAW_TYPE::ARRAYS)
            {
                // idk lol
            }
            else
            {
                for (int strip = 0; strip < this->info.NUM_STRIPS; ++strip)
                    glDrawElements(GL_TRIANGLE_STRIP, this->info.NUM_TRIS_PER_STRIP + 2, GL_UNSIGNED_INT, reinterpret_cast<const void*>(sizeof(unsigned int) * (this->info.NUM_TRIS_PER_STRIP + 2) * strip));
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

    void VertexArray::GenArray(const unsigned int n, unsigned int* array) { glGenVertexArrays(static_cast<GLsizei>(n), array); }

    void VertexArray::SetVertexAttributeData(const int index, const int n, const unsigned int type, const bool normalized, const int stride, const void* data)
    {
        glEnableVertexAttribArray(index); // enable the vertex attribute at the specific index/location first
        glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(n), static_cast<GLenum>(type), normalized, static_cast<GLsizei>(stride), data);
    }

    void VertexArray::Bind() const { glBindVertexArray(this->ID); }

    void VertexArray::Unbind() const { glBindVertexArray(0); }
}