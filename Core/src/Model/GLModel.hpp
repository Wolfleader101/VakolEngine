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

		void AddMesh(const std::shared_ptr<GLVertexArray>& VAO, const std::shared_ptr<GLShader>& shader);

	private:
		std::map<std::shared_ptr<GLMesh>, std::shared_ptr<GLMaterial>> meshes;
	};
}