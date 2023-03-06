#pragma once

#include "Buffer.hpp"

namespace Vakol::Model
{
	class GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer(const std::vector<Vertex>& vertices);

		~GLVertexBuffer() override;

		void Bind() const override;
		void Unbind() const override;


	private:
		unsigned int id = 0;
	};

	class GLIndexBuffer : public IndexBuffer
	{
	public:
		GLIndexBuffer(const std::vector<unsigned int>& indices);

		~GLIndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

	private:
		unsigned int id = 0;
	};

	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

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

		std::shared_ptr<GLVertexBuffer> vertexBuffer = nullptr;
		std::shared_ptr<GLIndexBuffer> indexBuffer = nullptr;
	};
}