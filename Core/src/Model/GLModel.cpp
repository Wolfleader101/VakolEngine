#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <iostream>

#include "GLModel.hpp"

namespace Vakol::Model
{
    bool MeshCompare::operator()(const GLMesh& left, const GLMesh& right) const { return left.GetID() < right.GetID(); }

	GLMesh::GLMesh(const GLVertexArray& VAO)
	{
		this->VAO = VAO;
	}

	GLMesh::~GLMesh()
	{
		this->VAO.~GLVertexArray();
	}

	const unsigned int GLMesh::GetID() const { return this->VAO.GetID(); }

	void GLMesh::Draw() const
	{
		this->VAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, this->VAO.GetVertexCount());
		this->VAO.Unbind();
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

	void GLModel::AddMesh(const GLVertexArray& VAO) 
	{ 
		meshes.push_back(GLMesh(VAO));
	}

	//void GLModel::AddMesh(const std::shared_ptr<GLVertexArray>& VAO, const std::shared_ptr<GLShader>& shader)
	//{
 //       auto mesh = GLMesh(VAO);


	//}
}
