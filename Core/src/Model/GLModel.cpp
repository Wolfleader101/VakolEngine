#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "GLModel.hpp"

namespace Vakol::Model
{
	GLMesh::GLMesh(const std::shared_ptr<GLVertexArray>& VAO)
	{
		this->VAO = VAO;
	}

	GLMesh::~GLMesh()
	{
		this->VAO->~GLVertexArray();
	}

	void GLMesh::Draw() const
	{
		this->VAO->Bind();
		glDrawArrays(GL_TRIANGLES, 0, this->VAO->GetVertexCount());
		this->VAO->Unbind();
	}

	GLModel::~GLModel()
	{
		for (const auto& it : meshes)
		{
			it.first->~GLMesh();
			it.second->~GLMaterial();
		}
	}

	void GLModel::Draw() const
	{
		for (const auto& it : meshes)
		{
			it.second->Bind();

			it.first->VAO->Bind();
		}
	}

	void GLModel::AddMesh(const std::shared_ptr<GLVertexArray>& VAO, const std::shared_ptr<GLShader>& shader)
	{
		auto pair = std::make_pair <std::shared_ptr<GLMesh>, std::shared_ptr<GLMaterial>>;

		meshes.emplace(pair(std::make_shared<GLMesh>(VAO), std::make_shared<GLMaterial>(shader)));
	}
}
