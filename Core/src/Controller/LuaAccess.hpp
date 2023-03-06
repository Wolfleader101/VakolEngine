#pragma once

#include <sol/sol.hpp>

namespace Vakol::Controller {
    void RegisterGameConfig(sol::state& lua);
    void RegisterLogger(sol::state& lua);
}  // namespace Vakol::Controller