#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
        glDrawElements(GL_TRIANGLES, this->VAO.GetIndexCount(), GL_UNSIGNED_INT, 0);
		this->VAO.Unbind();
	}

	GLModel::~GLModel()
	{
		for (const auto& it : meshes)
		{
			it.first.~GLMesh();
			it.second.~GLMaterial();
		}
	}

	void GLModel::Draw(const DRAW_TYPE type) const 
	{ 
		for (const auto& it : meshes) 
		{
            if (type == DRAW_TYPE::MATERIAL)
				it.second.Bind();
            else
				it.first.Draw();    
		}
	}

	void GLModel::AddMesh(const GLVertexArray& VAO, const GLShader& shader)
	{ 
		meshes.insert(std::pair<GLMesh, GLMaterial>(VAO, shader));
	}
}
