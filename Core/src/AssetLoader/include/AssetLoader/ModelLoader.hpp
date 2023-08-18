#pragma once

#include "Rendering/Assets/Model.hpp"

namespace Vakol
{
    Rendering::Assets::Model ImportModel(const char* path, float scale, bool& success);
}
