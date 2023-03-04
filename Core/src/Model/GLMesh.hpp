#pragma once

#include "Mesh.hpp"

namespace Vakol::Model
{
	class GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer(const std::vector<Vertex>& vertices);
		GLVertexBuffer(const std::vector<Math::Vec3>& vertices);

		~GLVertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
	};

	class GLIndexBuffer : public IndexBuffer
	{
	public:
		GLIndexBuffer(const std::vector<unsigned int>& indices);

		~GLIndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;
	};

	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray(const Mesh& mesh);
		GLVertexArray(const std::vector<Math::Vec3>& vertices, const std::vector<unsigned int>& indices);

		~GLVertexArray() override;

		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id;

		std::shared_ptr<GLVertexBuffer> vertexBuffer;
		std::shared_ptr<GLIndexBuffer> indexBuffer;
	};
}