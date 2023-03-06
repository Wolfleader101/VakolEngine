#pragma once

#include <map>

#include "Model.hpp"

#include "GLMaterial.hpp"
#include "GLBuffer.hpp"

namespace Vakol::Model
{

	class GLMesh : public Mesh
	{
	public:
        GLMesh() = default;
		GLMesh(const GLVertexArray& VAO);

		~GLMesh() override;

		void Draw() const override;

		const unsigned int GetID() const override;

	private:
		GLVertexArray VAO;
	};

	struct MeshCompare
	{
        bool operator()(const GLMesh& left, const GLMesh& right) const;
	};

	class GLModel : public Model
	{
	public:
		~GLModel() override;

		void Draw(const DRAW_TYPE type) const override;

		void AddMesh(const GLVertexArray& VAO, const GLShader& shader);

		std::map<GLMesh, GLMaterial, MeshCompare> meshes;
	};
}