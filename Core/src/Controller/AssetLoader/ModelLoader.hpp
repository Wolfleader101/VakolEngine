#pragma once

#include <string>

#include <Model/Assets/Model.hpp>

namespace Vakol::Controller
{
	
	struct ModelLoader
	{
		using Model = Vakol::Model::Assets::Model;
		Model LoadModel(const std::string& path);
	};
}
