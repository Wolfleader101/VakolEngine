#pragma once

#include <sol/sol.hpp>

#include "Application.hpp"
#include "Logger.hpp"
#include "Scene.hpp"

namespace Vakol::Controller {
    void RegisterAssetLoader(sol::state& lua);
    void RegisterLogger(sol::state& lua);
    void RegisterApplication(sol::state& lua, Application* app);
    void RegisterEntity(sol::state& lua);
    void RegisterECS(sol::state& lua);
    void RegisterGUIWindow(sol::state& lua, View::Window* applicationWindow);
    void RegisterRenderer(sol::state& lua);
    void RegisterPhysics(sol::state& lua);
    void RegisterScene(sol::state& lua);
    void RegisterMath(sol::state& lua);
}  // namespace Vakol::Controller