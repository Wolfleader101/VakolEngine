#pragma once

#include <string>

#include "Model/Assets/Model.hpp"
#include "Controller/Animator.hpp"

namespace Vakol::Controller
{
	std::pair<Model::Assets::Model, Animator> LoadModel(const std::string& path, float scale, bool animated);
}