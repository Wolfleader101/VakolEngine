#pragma once

#include <unordered_map>

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

		const unsigned int GetID() const override;

	private:
		std::shared_ptr<GLVertexArray> VAO = nullptr;
	};

	class GLModel : public Model
	{
	public:
		~GLModel() override;

		void Draw() const override;

		void AddMesh(const std::shared_ptr<GLVertexArray>& VAO);
        void AddMaterial(const std::shared_ptr<GLShader>& shader);

	private:
		std::unordered_map<unsigned int, std::shared_ptr<GLMesh>> meshes;
        std::unordered_map<unsigned int, std::shared_ptr<GLMaterial>> materials;
	};
}