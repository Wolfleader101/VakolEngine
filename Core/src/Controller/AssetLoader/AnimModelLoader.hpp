#pragma once

#include <string>

#include "Model/Assets/Model.hpp"

namespace Vakol::Controller
{
	using Model::Assets::Model;

	Model LoadAnimatedModel(std::string&& path);
}