#pragma once

#include "Math/Vector2.hpp"
#include "Math/Vector3.hpp"

#include <vector>

typedef unsigned int uint;

struct Vertex
{
	Vector3 position;
	//ADD THIS LATER//Vector3 normal;
	Vector2 uv;
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