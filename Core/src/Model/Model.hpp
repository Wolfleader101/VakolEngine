#pragma once

#include "Material.hpp"

namespace Vakol::Model
{
    enum DRAW_TYPE { MESH, MATERIAL };

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

		virtual void Draw(const DRAW_TYPE type) const = 0;
	};
}