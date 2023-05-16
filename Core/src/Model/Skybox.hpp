#pragma once

#include <string>
#include <memory>

#include <Model/Shader.hpp>
#include <Model/VertexArray.hpp>

namespace Vakol::Model
{
	struct Skybox
	{
		Skybox()
		{
			shader = std::make_shared<Shader>("coreAssets/shaders/skybox.prog");
		}

		std::shared_ptr<Shader> shader = nullptr;

		unsigned int CUBEMAP_ID = 0;
	};
}