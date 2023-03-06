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

		void Draw() const override;

		void AddMesh(const GLVertexArray& VAO);
		//void AddMesh(const std::shared_ptr<GLVertexArray>& VAO, const std::shared_ptr<GLShader>& shader);

	private:
        std::vector<GLMesh> meshes = std::vector<GLMesh>();
		//std::map<GLMesh, GLMaterial, MeshCompare> meshes;
	};
}