#pragma once

#include "Model.hpp"
#include "GLMaterial.hpp"

#include "GLBuffer.hpp"

#include <map>

namespace Vakol::Model
{
	class GLMesh : public Mesh
	{
	public:
		GLMesh(const std::shared_ptr<GLVertexArray>& VAO);

		~GLMesh() override;

		void Draw() const override;

		std::shared_ptr<GLVertexArray> VAO;
	};

	class GLModel : public Model
	{
	public:
		~GLModel() override;

		void Draw() const override;

		void AddMesh(const std::shared_ptr<GLVertexArray>& VAO, const std::string& shader);

	private:
		std::map<GLMesh, GLMaterial> meshes;
	};
}