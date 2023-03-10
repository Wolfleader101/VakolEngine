#pragma once

#include <sol/sol.hpp>

#include "Application.hpp"
#include "Logger.hpp"
#include "Scene.hpp"

namespace Vakol::Controller {
    void RegisterGameConfig(sol::state& lua);
    void RegisterLogger(sol::state& lua);
    void RegisterApplication(sol::state& lua, Application* app);
    void RegisterECS(sol::state& lua);
    void RegisterWindow(sol::state& lua);
    void RegisterRenderer(sol::state& lua);
    void RegisterPhysics(sol::state& lua);
}  // namespace Vakol::Controller