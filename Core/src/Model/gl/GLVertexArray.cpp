#include <glad/glad.h>

#include <Model/gl/GLVertexArray.hpp>
#include <Controller/Logger.hpp>

constexpr int NUM_PATCH_PTS = 4;

namespace Vakol::Model {

    GLVertexArray::GLVertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : VertexArray(vertices, indices) 
    {
        this->GenArray(1, &this->VAO_ID);
        this->Bind();

        this->GenBuffer(1, &this->VBO_ID);
        this->BindBuffer(GL_ARRAY_BUFFER, this->VBO_ID);
        glBufferData(GL_ARRAY_BUFFER, GetVertexCount() * sizeof(Vertex), this->m_vertices.data(), GL_STATIC_DRAW);

        this->GenBuffer(1, &this->EBO_ID);
        this->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexCount() * sizeof(unsigned int), this->m_indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

        //glEnableVertexAttribArray(5);
        //glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        //glEnableVertexAttribArray(6);
        //glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

        this->Unbind();
    }

    GLVertexArray::GLVertexArray(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) : VertexArray(vertices, indices)
    {

    }

    GLVertexArray::GLVertexArray(const std::vector<float>& vertices) : VertexArray(vertices)
    {
        this->GenArray(1, &this->VAO_ID);
        this->Bind();

        this->GenBuffer(1, &this->VBO_ID);
        this->BindBuffer(GL_ARRAY_BUFFER, this->VBO_ID);
        glBufferData(GL_ARRAY_BUFFER, this->m_verts.size() * sizeof(float), this->m_verts.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

        glPatchParameteri(GL_PATCH_VERTICES, NUM_PATCH_PTS);

        this->Unbind();
    }


    void GLVertexArray::DrawArrays() const 
    {
        this->Bind();

        glDrawArrays(GL_TRIANGLES, 0, this->GetVertexCount());

        this->Unbind();
    }

    void GLVertexArray::DrawElements() const 
    {
        this->Bind();

        glDrawElements(GL_TRIANGLES, this->GetIndexCount(), GL_UNSIGNED_INT, 0);

        this->Unbind();
    }

    void GLVertexArray::DrawTriangleStrips() const 
    {
        this->Bind();

        for (int strip = 0; strip < this->NUM_STRIPS; ++strip)
            glDrawElements(GL_TRIANGLE_STRIP, this->NUM_TRIS_PER_STRIP + 2, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * (NUM_TRIS_PER_STRIP + 2) * strip));

        this->Unbind();
    }

    void GLVertexArray::DrawQuadPatches() const
    {
        this->Bind();

        glDrawArrays(GL_PATCHES, 0, NUM_PATCHES * NUM_VERTS_PER_PATCH);

        this->Unbind();
    }

    void GLVertexArray::DrawArraysInstanced(const int amount) const 
    {
        this->Bind();

        glDrawArraysInstanced(GL_TRIANGLES, 0, this->GetVertexCount(), amount);

        this->Unbind();
    }

    void GLVertexArray::DrawElementsInstanced(const int amount) const 
    {
        this->Bind();

        glDrawElementsInstanced(GL_TRIANGLES, this->GetIndexCount(), GL_UNSIGNED_INT, 0, amount);

        this->Unbind();
    }

    void GLVertexArray::GenArray(const unsigned int n, unsigned int* array) {
        glGenVertexArrays(static_cast<GLsizei>(n), static_cast<GLuint*>(array));
    }

    void GLVertexArray::GenBuffer(const unsigned int n, unsigned int* buffer) {
        glGenBuffers(static_cast<GLsizei>(n), static_cast<GLuint*>(buffer));
    }

    void GLVertexArray::BindBuffer(const unsigned int type, const unsigned int buffer) {
        glBindBuffer(static_cast<GLenum>(type), static_cast<GLenum>(buffer));
    }

    GLVertexArray::~GLVertexArray() 
    {
        glDeleteVertexArrays(1, &this->VAO_ID);
        glDeleteBuffers(1, &this->VBO_ID);
        glDeleteBuffers(1, &this->EBO_ID);
    }

    void GLVertexArray::Bind() const { glBindVertexArray(this->VAO_ID); }

    void GLVertexArray::Unbind() const { glBindVertexArray(0); }
}