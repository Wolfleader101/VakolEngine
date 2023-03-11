#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLBuffer.hpp"

using Vakol::Model::Vertex;
using Vakol::Model::Vertex3;
using Vakol::Model::Vertex2N;
using Vakol::Model::Vertex2U;

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

	GLVertexArray::GLVertexArray(const std::vector<Vertex3>& vertices, const std::vector<unsigned int>& indices)
	{
		this->vertex_count = static_cast<unsigned int>(vertices.size());
		this->index_count = static_cast<unsigned int>(indices.size());

		unsigned int VBO, EBO;

		glGenVertexArrays(1, &this->id);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(this->id);
                
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex3), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)offsetof(Vertex3, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3), (void*)offsetof(Vertex3, uv));

		glBindVertexArray(0);
	}

	GLVertexArray::GLVertexArray(const std::vector<Vertex2N>& vertices, const std::vector<unsigned int>& indices)
	{
				this->vertex_count = static_cast<unsigned int>(vertices.size());
		this->index_count = static_cast<unsigned int>(indices.size());

		unsigned int VBO, EBO;

		glGenVertexArrays(1, &this->id);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(this->id);
                
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex2N), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2N), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2N), (void*)offsetof(Vertex2N, normal));

		glBindVertexArray(0);
	}
	
	GLVertexArray::GLVertexArray(const std::vector<Vertex2U>& vertices, const std::vector<unsigned int>& indices)
	{
		this->vertex_count = static_cast<unsigned int>(vertices.size());
		this->index_count = static_cast<unsigned int>(indices.size());

		unsigned int VBO, EBO;

		glGenVertexArrays(1, &this->id);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(this->id);
                
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex2U), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2U), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2U), (void*)offsetof(Vertex2U, uv));

		glBindVertexArray(0);
	}

	GLVertexArray::GLVertexArray(const std::vector<Vec3>& vertices, const std::vector<unsigned int>& indices)
	{
		this->vertex_count = static_cast<unsigned int>(vertices.size());
		this->index_count = static_cast<unsigned int>(indices.size());

		unsigned int VBO, EBO;

		glGenVertexArrays(1, &this->id);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(this->id);
                
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vec3), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)0);

		glBindVertexArray(0);
	}

	GLVertexArray::GLVertexArray(const std::vector<float>& vertices)
	{
		this->vertex_count = static_cast<unsigned int>(vertices.size());

		unsigned int VBO;

		glGenVertexArrays(1, &this->id);
		glGenBuffers(1, &VBO);

		glBindVertexArray(this->id);
                
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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