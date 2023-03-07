#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLBuffer.hpp"

#include <iostream>

namespace Vakol::Model
{
	GLVertexArray::GLVertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		this->vertex_count = static_cast<unsigned int>(vertices.size());
		this->index_count = static_cast<unsigned int>(indices.size());

		unsigned int VBO, EBO;

		glGenVertexArrays(1, &this->id);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(this->id);
                
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	GLVertexArray::~GLVertexArray()
	{
		//glDeleteVertexArrays(1, &this->id);
	}

	void GLVertexArray::Bind() const
	{
		glBindVertexArray(this->id);
	}

	void GLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	const unsigned int GLVertexArray::GetVertexCount() const
	{
		return this->vertex_count;
	}

	const unsigned int GLVertexArray::GetIndexCount() const
	{
		return this->index_count;
	}

	const unsigned int GLVertexArray::GetID() const 
	{ 
		return this->id;
	}
}