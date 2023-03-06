#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLBuffer.hpp"

namespace Vakol::Model
{
	GLVertexArray::GLVertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	{
		this->vertex_count = static_cast<unsigned int>(vertices.size());
		this->index_count = static_cast<unsigned int>(indices.size());

		glGenVertexArrays(1, &this->id);
		glBindVertexArray(this->id);

		this->vertexBuffer = std::make_shared<GLVertexBuffer>(vertices);
		this->indexBuffer = std::make_shared<GLIndexBuffer>(indices);

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
		glDeleteVertexArrays(1, &this->id);
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

	/*********************************************/
	/*********************************************/

	GLVertexBuffer::GLVertexBuffer(const std::vector<Vertex>& vertices)
	{
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &this->id);
	}

	void GLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}

	void GLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/**************************************/
	/**************************************/

	GLIndexBuffer::GLIndexBuffer(const std::vector<unsigned int>& indices)
	{
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		glDeleteBuffers(1, &this->id);
	}

	void GLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	}

	void GLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}