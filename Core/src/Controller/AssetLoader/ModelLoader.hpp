#pragma once

#include <string>
#include <Model/Assets/Model.hpp>

namespace Vakol::Controller
{
    Vakol::Model::Assets::Model LoadModel(const std::string& path);
}