#pragma once

#include <string>
#include <memory>

namespace Vakol::Model 
{
	class Texture 
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind() const = 0;

		virtual void Unbind() const = 0;

		virtual unsigned int GetID() const = 0;

		static std::shared_ptr<Texture> Create(const std::string& name);
	};
}