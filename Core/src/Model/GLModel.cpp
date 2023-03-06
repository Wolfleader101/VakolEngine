#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <iostream>

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

	const unsigned int GLMesh::GetID() const { return this->VAO->GetID(); }

	void GLMesh::Draw() const
	{
		this->VAO->Bind();
		glDrawArrays(GL_TRIANGLES, 0, this->VAO->GetVertexCount());
		this->VAO->Unbind();
	}

	GLModel::~GLModel()
	{
		//for (const auto& it : meshes)
		//{
		//	//it.first->~GLMesh();
		//	//it.second->~GLMaterial();
		//}
	}

	void GLModel::Draw() const
	{
		//for (const auto& it : meshes)
		//{
		//	//it.second->Bind();

		//	//it.first->VAO->Bind();
		//}
	}

	void GLModel::AddMesh(const std::shared_ptr<GLVertexArray>& VAO)
	{
        auto mesh = std::make_shared<GLMesh>(VAO);

        meshes.insert(std::pair<unsigned int, std::shared_ptr<GLMesh>>(mesh->GetID(), mesh));
	}

	void GLModel::AddMaterial(const std::shared_ptr<GLShader>& shader)
	{
        auto material = std::make_shared<GLMaterial>(shader);

		materials.insert(std::pair<unsigned int, std::shared_ptr<GLMaterial>>(material->GetID(), material));
	}
}
