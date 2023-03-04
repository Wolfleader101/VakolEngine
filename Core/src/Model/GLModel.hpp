#pragma once

#include "Model.hpp"

namespace Vakol::Model
{
	class GLModel : public Model
	{
	public:
		~GLModel() override;

		void Draw() const override;

		void AddMesh(const GLMesh& mesh);

	private:
		std::vector<GLMesh& 
	};
}