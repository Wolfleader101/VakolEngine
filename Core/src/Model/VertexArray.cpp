#include "VertexArray.hpp"

#include <glad/glad.h>
#include <Controller/Logger.hpp>

#include <iostream>

#define VERBOSE_DEBUG 0

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

        VK_ASSERT(arr_size % elements == 0, "\n\nVector size is not divisible by the number of vertex elements. Double check your size input.");

        output.reserve(arr.size());

        for (int i = 0; i < arr_size; i += elements)
        {
            const auto& position = arr.at(i).position;
            const auto& normal = arr.at(i + 1).normal;
            const auto& uv = arr.at(i + 2).uv;
        }

        //arr.~vector(); // don't need that no more

        return output;
    }

    VertexArray::VertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const int size) : vertices(std::move(vertices)), indices(std::move(indices))
    {
        auto t_elements = static_cast<int>(size / sizeof(float)); // total elements remaining
        auto n_elements = 0; // number of elements used

        VK_TRACE("Vertex Size = {0}", size);
        VK_TRACE("Vertex Elements = {0} (number of floats)", n_elements);

        std::cout << std::endl;
        
        this->n_vertices = static_cast<int>(this->vertices.size());
        this->n_indices = static_cast<int>(this->indices.size());

        VK_ASSERT(n_vertices > 0, "\n\nCannot create empty vertex array.");
        VK_ASSERT(t_elements >= 3, "\n\nYou'll need at least 3 vertex elements (float values) to create a functional vertex array");

        this->GenArray(1, &this->ID);
        this->VBO.GenBuffer(1);
        if (n_indices > 0) this->EBO.GenBuffer(1);

        this->Bind();

        this->VBO.Bind(GL_ARRAY_BUFFER);
        this->VBO.SetData(n_vertices * sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
        
        if (n_indices > 0)
        {
            this->EBO.Bind(GL_ELEMENT_ARRAY_BUFFER);
            this->EBO.SetData(n_indices * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);
        }

        this->EnableVertexAttribArray(0);
        this->SetVertexAttribData(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0);
        t_elements -= 3;
        n_elements += 3;

        if (t_elements >= 2)
        {
            this->EnableVertexAttribArray(1);

            if (t_elements >= 3)
            {
                this->SetVertexAttribData(1, 3, GL_FLOAT, GL_FALSE, size, (void*)(n_elements * sizeof(float)));
                t_elements -= 3;
                n_elements += 3;
                VK_TRACE("Vertex Normals");
            }
            else
            {
                this->SetVertexAttribData(1, 2, GL_FLOAT, GL_FALSE, size, (void*)(n_elements * sizeof(float)));
                t_elements -= 2;
                n_elements += 2;
                VK_TRACE("Vertex UVs");
            }
        }

        if (t_elements >= 3)
        {
            this->EnableVertexAttribArray(2);
            this->SetVertexAttribData(2, 2, GL_FLOAT, GL_FALSE, size, (void*)(n_elements * sizeof(float)));
            t_elements -= 2;
            n_elements += 2;
        }

        if (t_elements >= 3)
        {
            this->EnableVertexAttribArray(3);
            this->SetVertexAttribData(3, 3, GL_FLOAT, GL_FALSE, size, (void*)(n_elements * sizeof(float)));
            t_elements -= 3;
            n_elements += 3;
        }

        VK_ASSERT(t_elements == 0, "\n\nLooks like someone forgot to use all of their vertex data!");

        this->Unbind();
        this->VBO.Unbind(GL_ARRAY_BUFFER); // turns out this is perfectly legal, don't unbind EBO though!
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &this->ID);
    }

    void VertexArray::GenArray(const unsigned int n, unsigned int* array)
    {
        glGenVertexArrays(static_cast<GLsizei>(n), static_cast<GLuint*>(array));
    }

    void VertexArray::EnableVertexAttribArray(const unsigned int location)
    {
        glEnableVertexAttribArray(static_cast<GLuint>(location));
    }

    void VertexArray::SetVertexAttribData(const int index, const int n, const unsigned int type, const bool normalized, const int stride, const void* data)
    {
        glEnableVertexAttribArray(index); // enable the vertex attribute at the specific index/location first
        glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(n), static_cast<GLenum>(type), normalized, static_cast<GLsizei>(stride), data);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(this->ID);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
}

