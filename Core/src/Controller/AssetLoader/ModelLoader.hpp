#pragma once

#include <string>

#include "Model/Assets/Model.hpp"

namespace Vakol::Controller
{
	Model::Assets::Model LoadModel(const std::string& path, const float scale, bool animated);
}