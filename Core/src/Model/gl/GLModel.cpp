#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Controller/Logger.hpp>

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

	void GLMesh::Draw(const unsigned int type) const
	{
		this->VAO.Bind();

		if (type == GL_INDEX_ARRAY)
        	glDrawElements(GL_TRIANGLES, this->VAO.GetIndexCount(), GL_UNSIGNED_INT, 0);
		else if (type == GL_VERTEX_ARRAY)
			glDrawArrays(GL_TRIANGLES, 0, this->VAO.GetVertexCount());

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
		glDepthFunc(static_cast<GLenum>(depthCompare));
			
		if (type == GL_SHADER)
			meshes.begin()->second.Bind(type); // Bind Shader
		else // if not binding the shader
		{
			// Bind Texture
			meshes.begin()->second.Bind(type);

			// Determine if we want to draw mesh using indices, or just vertices.
			// Indexed vertices usually has better performance results due to re-use of vertices
			if (type == GL_TEXTURE_2D)
				meshes.begin()->first.Draw(GL_INDEX_ARRAY);
			else if (type == GL_TEXTURE_CUBE_MAP)
				meshes.begin()->first.Draw(GL_VERTEX_ARRAY);
		}

		glDepthFunc(GL_LESS); // set depth back to normal *regardless if the depth has been shifted or not*
	}

	void GLModel::AddMesh(const GLMesh& mesh, const GLMaterial& material)
	{ 
		meshes.insert(std::make_pair(mesh, material));
	}
}