#pragma once

#include <string>
#include <memory>

namespace Vakol::Model {

struct Texture {
	virtual ~Texture() = default;

	virtual void Bind() = 0;

	virtual void Unbind() = 0;

	virtual unsigned int GetID() = 0;

	static std::shared_ptr<Texture> Create(const std::string& name);
};
}