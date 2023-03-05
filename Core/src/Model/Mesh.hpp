#pragma once

#include "Model/Math/Vector2.hpp"
#include "Model/Math/Vector3.hpp"

#include <vector>

typedef unsigned int uint;

struct Vertex
{
	Vakol::Model::Math::Vector3 position;
	//ADD THIS LATER//Vector3 normal;
	Vakol::Model::Math::Vector2 uv;
};

struct Mesh
{
	Mesh() = default;
	~Mesh() { this->Clear(); }

	void Clear()
	{
		vertices.clear();
		indices.clear();
	}

	std::vector<Vertex> vertices;
	std::vector<uint> indices;

private:
	uint id;
};