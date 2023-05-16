#pragma once

#include <memory>

#include <Model/Shader.hpp>

namespace Vakol::Model
{
	struct Skybox
	{
		Skybox() = default;

		void Init();
		void Draw(const glm::mat4& projection, const glm::mat4& view) const;

	private:
		std::shared_ptr<Shader> shader = nullptr;
		unsigned int CUBEMAP_ID = 0;
	};
}