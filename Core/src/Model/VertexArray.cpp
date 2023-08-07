#include "VertexArray.hpp"

#include <glad/glad.h>

#include <Controller/Logger.hpp>
#include <iostream>

#define VERBOSE_FLOAT_DEBUG 0
#define VERBOSE_VERTEX_DEBUG 0

namespace Vakol::Model
{
    std::vector<Vertex> Convert(std::vector<float> &arr, const size_t size)
    {
        const auto arr_size = arr.size();
        const auto elements = static_cast<int>(size / sizeof(float));

#if VERBOSE_FLOAT_DEBUG
        VK_TRACE("Converting vector of Vertex to vector of floats.");

        std::cout << std::endl;

        VK_TRACE("Vector info:");
        std::cout << std::endl;
        VK_TRACE("Vector Size = {0}", arr_size);
        VK_TRACE("Vertex Size = {0}", size);
        VK_TRACE("Vertex Elements = {0} (number of floats)", elements);

        std::cout << std::endl;
#endif

        if (arr.empty())
        {
            VK_CRITICAL("Convert() - Cannot convert empty vector");
            return {};
        }

        if (!(elements == 3 || elements == 5 || elements == 8 || elements == 14 || elements == 22))
        {
            VK_CRITICAL("Convert() - Unknown Vertex size.");
            return {};
        }

        std::vector<Vertex> output;

        output.reserve(arr_size / elements);

        Vertex vertex{};

        for (auto i = static_cast<size_t>(0); i < arr_size; i += elements)
        {
            vertex.position = {arr.at(i), arr.at(i + 1), arr.at(i + 2)};

            if (elements >= 5 && elements < 8)
                vertex.uv = {arr.at(i + 3), arr.at(i + 4)};

            if (elements >= 8)
            {
                vertex.normal = {arr.at(i + 3), arr.at(i + 4), arr.at(i + 5)};
                vertex.uv = {arr.at(6), arr.at(7)};
            }

            if (elements >= 14)
            {
                vertex.tangent = {arr.at(8), arr.at(9), arr.at(10)};
                vertex.bitangent = {arr.at(11), arr.at(12), arr.at(13)};
            }

            if (elements >= 22)
            {
                vertex.bone_ids[0] = static_cast<int>(arr.at(14));
                vertex.bone_ids[1] = static_cast<int>(arr.at(15));
                vertex.bone_ids[2] = static_cast<int>(arr.at(16));
                vertex.bone_ids[3] = static_cast<int>(arr.at(17));

                vertex.bone_weights[0] = arr.at(18);
                vertex.bone_weights[1] = arr.at(19);
                vertex.bone_weights[2] = arr.at(20);
                vertex.bone_weights[3] = arr.at(21);
            }

            if (i % elements == 0)
            {
                // VK_TRACE("{0} {1} {2}", vertex.position.x, vertex.position.y, vertex.position.z);
                // VK_TRACE("{0} {1}", vertex.uv.x, vertex.uv.y);
                output.push_back(vertex);
            }
        }

        arr.~vector();

        return output;
    }

    VertexArray::VertexArray(std::vector<Vertex> &&vertices, std::vector<unsigned int> &&indices, size_t size)
        : vertices(vertices), indices(indices)
    {
        int total_elements = static_cast<int>(size / sizeof(float));

        this->n_vertices = static_cast<int>(this->vertices.size());
        this->n_indices = static_cast<int>(this->indices.size());

        if (size != sizeof(Vertex))
            size = sizeof(Vertex);

        if (n_vertices <= 0)
        {
            VK_CRITICAL("VertexArray::VertexArray - Cannot create empty vertex array.");
            return;
        }

#if VERBOSE_VERTEX_DEBUG
        std::cout << std::endl;

        VK_TRACE("Vector info:");

        std::cout << std::endl;

        VK_TRACE("Vector Size = {0}", n_vertices);
        VK_TRACE("Vertex Size = {0}", size);
        VK_TRACE("Vertex Elements = {0} (number of floats)", total_elements);

        std::cout << std::endl;
#endif

        GenArray(1, &this->ID);
        glGenBuffers(1, &this->VBO);
        if (n_indices > 0)
            glGenBuffers(1, &this->EBO); // EBO is optional

        if (!(this->VBO != 0 && (n_indices > 0) ? this->EBO != 0 : true))
        {
            VK_CRITICAL("VertexArray::VertexArray - Failed to generate one or more buffers!");
            return;
        }

        this->Bind();

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(n_vertices * sizeof(Vertex)), this->vertices.data(),
                     GL_STATIC_DRAW);

        if (n_indices > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(n_indices * sizeof(unsigned int)),
                         this->indices.data(), GL_STATIC_DRAW);
        }

        if (total_elements >= 3)
        {
            SetVertexAttributeData(0, 3, GL_FLOAT, GL_FALSE, size, nullptr);
            total_elements -= 3;
        }

        if (total_elements >= 2 && total_elements <= 3)
        {
            SetVertexAttributeData(1, 2, GL_FLOAT, GL_FALSE, size,
                                   reinterpret_cast<const void *>(offsetof(Vertex, uv)));
            total_elements -= 2;
        }

        if (total_elements >= 3)
        {
            SetVertexAttributeData(1, 3, GL_FLOAT, GL_FALSE, size,
                                   reinterpret_cast<const void *>(offsetof(Vertex, normal)));
            total_elements -= 3;
        }

        if (total_elements >= 2)
        {
            SetVertexAttributeData(2, 2, GL_FLOAT, GL_FALSE, size,
                                   reinterpret_cast<const void *>(offsetof(Vertex, uv)));
            total_elements -= 2;
        }

        if (total_elements >= 3)
        {
            SetVertexAttributeData(3, 3, GL_FLOAT, GL_FALSE, size,
                                   reinterpret_cast<const void *>(offsetof(Vertex, tangent)));
            total_elements -= 3;
        }

        if (total_elements >= 3)
        {
            SetVertexAttributeData(4, 3, GL_FLOAT, GL_FALSE, size,
                                   reinterpret_cast<const void *>(offsetof(Vertex, bitangent)));
            total_elements -= 3;
        }

        if (total_elements >= 4)
        {
            SetVertexAttributeIData(5, 4, GL_INT, size, reinterpret_cast<const void *>(offsetof(Vertex, bone_ids)));
            total_elements -= 4;
        }

        if (total_elements >= 4)
        {
            SetVertexAttributeData(6, 4, GL_FLOAT, GL_FALSE, size,
                                   reinterpret_cast<const void *>(offsetof(Vertex, bone_weights)));
            total_elements -= 4;
        }

        if (total_elements != 0)
        {
            VK_CRITICAL("VertexArray::VertexArray - Reserved Vertex data was not fully allocated.");
            return;
        }

        glPatchParameteri(GL_PATCH_VERTICES, NUM_PATCH_PTS);

        Unbind();
        glBindBuffer(GL_ARRAY_BUFFER, 0); // turns out this is perfectly legal, don't unbind EBO though!
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &this->ID);
    }

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
                glDrawElementsInstanced(GL_TRIANGLES, this->n_indices, GL_UNSIGNED_INT, nullptr,
                                        this->info.INSTANCE_AMOUNT);
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
                    glDrawElements(GL_TRIANGLE_STRIP, this->info.NUM_TRIS_PER_STRIP + 2, GL_UNSIGNED_INT,
                                   reinterpret_cast<const void *>(sizeof(unsigned int) *
                                                                  (this->info.NUM_TRIS_PER_STRIP + 2) * strip));
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

        Unbind();
    }

    void VertexArray::GenArray(const unsigned int n, unsigned int *array)
    {
        glGenVertexArrays(static_cast<GLsizei>(n), array);
    }

    void VertexArray::SetVertexAttributeData(const unsigned int index, const int n, const unsigned int type,
                                             const bool normalized, const size_t stride, const void *data)
    {
        glEnableVertexAttribArray(index); // enable the vertex attribute at the specific index/location first
        glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(n), static_cast<GLenum>(type), normalized,
                              static_cast<GLsizei>(stride), data);
    }

    void VertexArray::SetVertexAttributeIData(const unsigned int index, const int n, const unsigned int type,
                                              const size_t stride, const void *data)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribIPointer(static_cast<GLuint>(index), static_cast<GLint>(n), static_cast<GLenum>(type),
                               static_cast<GLsizei>(stride), data);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(this->ID);
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }
} // namespace Vakol::Model