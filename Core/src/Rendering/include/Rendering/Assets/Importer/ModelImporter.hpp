#pragma once

#include "../Model.hpp"

namespace Vakol::Rendering::Assets::Importer
{
    Model ImportModel(const char* path, float scale, bool& success);
}
