#pragma once

#include "Model.hpp"
#include "GLMaterial.hpp"

namespace Vakol::Model
{
	class GLMesh : public Mesh
	{
		~GLMesh() override;

		void Draw() const override;
	};

	class GLModel : public Model
	{
	public:
		~GLModel() override;

		void Draw() const override;

		void AddMesh(const GLMesh& mesh);
		void AddMaterial(const GLMaterial& material);

	private:
		std::vector<std::shared_ptr<GLMesh>> meshes;
		std::vector<std::shared_ptr<GLMaterial>> materials;
	};
}