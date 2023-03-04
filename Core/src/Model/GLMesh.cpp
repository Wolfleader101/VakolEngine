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
}