#pragma once

#include "../Buffer.hpp"

namespace Vakol::Model
{
	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray() = default;
		GLVertexArray(const std::vector<Vakol::Model::Vertex>& vertices, const std::vector<unsigned int>& indices);
		GLVertexArray(const std::vector<Vakol::Model::Vertex3>& vertices, const std::vector<unsigned int>& indices);
		GLVertexArray(const std::vector<Vakol::Model::Vertex2N>& vertices, const std::vector<unsigned int>& indices);
		GLVertexArray(const std::vector<Vakol::Model::Vertex2U>& vertices, const std::vector<unsigned int>& indices);
		GLVertexArray(const std::vector<Vakol::Model::Math::Vec3>& vertices, const std::vector<unsigned int>& indices);
		GLVertexArray(const std::vector<float>& vertices);

		~GLVertexArray() override;

		void Bind() const override;
		void Unbind() const override;

		const unsigned int GetVertexCount() const;
		const unsigned int GetIndexCount() const;
		const unsigned int GetID() const override;

	private:
		unsigned int id = 0;

		unsigned int vertex_count = 0;
		unsigned int index_count = 0;
	};
}