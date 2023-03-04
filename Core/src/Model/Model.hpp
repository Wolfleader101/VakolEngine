#pragma once

#include "Mesh.hpp"
#include "Material.hpp"

namespace Vakol::Model
{
	class Model
	{
	public:
		virtual ~Model() = default;

		virtual void Draw() const = 0;
	};
}