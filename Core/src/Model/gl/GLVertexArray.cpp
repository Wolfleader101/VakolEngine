#include <Model/gl/GLVertexArray.hpp>

#include <glad/glad.h>

namespace Vakol::Model 
{

	GLVertexArray::GLVertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : VertexArray(vertices, indices) 
    {
        this->GenArray(1, &this->VAO);
        this->Bind();

        this->GenBuffer(1, &this->VBO);
        this->BindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);

        this->GenBuffer(1, &this->EBO);
        this->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

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

        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

        this->Unbind();
    }

    void GLVertexArray::Draw() const
    {
        this->Bind();

        glDrawElements(GL_TRIANGLES, this->GetIndices(), GL_UNSIGNED_INT, 0);

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

    GLVertexArray::~GLVertexArray() {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);
    }

    void GLVertexArray::Bind() const { glBindVertexArray(this->VAO); }

    void GLVertexArray::Unbind() const { glBindVertexArray(0); }

    const unsigned int GLVertexArray::GetID() const { return this->VAO; }

    const unsigned int GLVertexArray::GetVertices() const { return static_cast<unsigned int>(this->vertices.size()); }

    const unsigned int GLVertexArray::GetIndices() const { return static_cast<unsigned int>(this->indices.size()); }
}