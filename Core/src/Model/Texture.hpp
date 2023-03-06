#pragma once

namespace Vakol::Model 
{
	class Texture 
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(const unsigned int) const = 0;

		virtual void Unbind() const = 0;

		virtual const unsigned int GetID() const = 0;
	};
}
