#pragma once

#include "Mesh.hpp"

namespace Vakol::Model
{
	class GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer(const std::vector<Vertex>& vertices);
		GLVertexBuffer(const std::vector<Vector3>& vertices);

		~GLVertexBuffer() = default;

		void Bind() override;
		void Unbind() override;

	private:
		unsigned int id;
	};

	class GLIndexBuffer : public IndexBuffer
	{
	public:
		GLIndexBuffer(const std::vector<unsigned int>& indices);

		~GLIndexBuffer() = default;

		void Bind() override;
		void Unbind() override;

	private:
		unsigned int id;
	};

	class GLVertexArray : public VertexArray
	{
	public:
		GLVertexArray(const Mesh& mesh);

		~GLVertexArray() override;

		void Bind() override;

		void Unbind() override;

	private:
		unsigned int id;

		std::shared_ptr<GLVertexBuffer> vertexBuffer;
		std::shared_ptr<GLIndexBuffer> indexBuffer;
	};

	class GLMesh
	{
	public:
		GLMesh() = default;

	private:
		GLVertexArray VAO;
		GLVertexBuffer VBO;
		GLIndexBuffer IBO;
	};
}