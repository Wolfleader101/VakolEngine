#pragma once

#include "Material.hpp"

namespace Vakol::Model
{
	class Mesh
	{
	public:
		virtual ~Mesh() = default;

		virtual void Draw() const = 0;

		virtual const unsigned int GetID() const = 0;
	};

	class Model
	{
	public:
		virtual ~Model() = default;

		virtual void Draw(const unsigned int type) const = 0;
	};
}