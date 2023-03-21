#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLBuffer.hpp"

using Vakol::Model::Vertex;

using Vakol::Model::Math::Vec3;

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

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

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

	void GLVertexArray::GenInstancedArrayBuffer(const unsigned int size, const unsigned int amount, void* ptr)
	{
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &ptr, GL_STATIC_DRAW);
	}

	void GLVertexArray::CreateInstancedVertexArray(const unsigned int VAO)
	{
		glBindVertexArray(VAO);

		// Assigned the vertex atribute location in the shader at 3
		// 1 Matrix (mat4) = 4 * vec4 (vec4 is the max data size of a vertex attribute)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::mat4)));

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::mat4)));

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::mat4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

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