#pragma once

#include <string>

#include <Controller/Animator.hpp>
#include <Model/Assets/Model.hpp>

#include <optional>

namespace Vakol::Controller
{
	std::pair<Model::Assets::Model, std::optional<Animator>> LoadModel(const std::string& path, float scale);
}