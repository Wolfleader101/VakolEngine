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
		//this->VAO.~GLVertexArray();
	}

	const unsigned int GLMesh::GetID() const { return this->VAO.GetID(); }

	void GLMesh::Draw() const
	{
		this->VAO.Bind();
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(this->VAO.GetIndexCount()), GL_UNSIGNED_INT, 0);
		this->VAO.Unbind();
	}

	GLModel::~GLModel()
	{
		//for (const auto& it : meshes)
		//{
		//	it.first.~GLMesh();
		//	it.second.~GLMaterial();
		//}
	}

	void GLModel::Draw(const unsigned int type) const 
	{ 
		if (type == GL_SHADER)
			meshes.begin()->second.Bind(GL_SHADER);
		else
		{
			meshes.begin()->second.Bind(GL_TEXTURE_2D);
			//glBindTexture(GL_TEXTURE_2D, meshes.begin()->second.texture);
			meshes.begin()->first.Draw();
		}
	}

	void GLModel::AddMesh(const GLMesh& mesh, const GLMaterial& material)
	{ 
		meshes.insert(std::make_pair(mesh, material));
	}

	void GLModel::AddTexture(const std::string& name)
	{
		//meshes.begin()->second.AddTexture(name);
	}
}
