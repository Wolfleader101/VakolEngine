#pragma once

#include <string>

#include <Model/Assets/Model.hpp>

using Model = Vakol::Model::Assets::Model;

namespace Vakol::Controller
{
	
	::Model LoadModel(const std::string& path);
	
}
