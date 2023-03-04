#pragma once

#include <string>
#include <memory>

namespace Vakol::Model 
{
	struct Texture 
	{
		virtual ~Texture() = default;

		virtual void Bind() const = 0;

		virtual void Unbind() const = 0;

		virtual unsigned int GetID() const = 0;

		static std::shared_ptr<Texture> Create(const std::string& name)
		{
			return std::make_shared<GLTexture>(name);
		}
	};
}