#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLMesh.hpp"

namespace Vakol::Model
{
	GLVertexArray::GLVertexArray(const Mesh& mesh)
	{
		glGenVertexArrays(1, &this->id);
		glBindVertexArray(this->id);

		this->vertexBuffer = std::make_shared<GLVertexBuffer>(mesh.vertices);
		this->indexBuffer = std::make_shared<GLIndexBuffer>(mesh.indices);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	GLVertexArray::GLVertexArray(const std::vector<Math::Vec3>& vertices, const std::vector<unsigned int>& indices)
	{
		glGenVertexArrays(1, &this->id);
		glBindVertexArray(this->id);

		this->vertexBuffer = std::make_shared<GLVertexBuffer>(vertices);
		this->indexBuffer = std::make_shared<GLIndexBuffer>(indices);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Math::Vec3), (void*)0);

		glBindVertexArray(0);
	}

	GLVertexArray::~GLVertexArray()
	{
		glDeleteVertexArrays(1, &this->id);
	}

	void GLVertexArray::Bind()
	{
		glBindVertexArray(this->id);
	}

	void GLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	/*********************************************/
	/*********************************************/

	GLVertexBuffer::GLVertexBuffer(const std::vector<Vertex>& vertices)
	{
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}

	GLVertexBuffer::GLVertexBuffer(const std::vector<Math::Vec3>& vertices)
	{
		glGenBuffers(1, &this->id);
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Math::Vec3), vertices.data(), GL_STATIC_DRAW);
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		glDeleteBuffers(1, &this->id);
	}

	void GLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}

	void GLVertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/**************************************/
	/**************************************/
}