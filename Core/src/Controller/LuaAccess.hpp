#pragma once

#include <sol/sol.hpp>

#include "Application.hpp"
#include "Scene.hpp"


namespace Vakol::Controller
{
    void RegisterAssetLoader(sol::state& lua);
    void RegisterLogger(sol::state& lua);
    void RegisterApplication(sol::state& lua, Application* app);
    void RegisterEntity(std::shared_ptr<LuaState>& state, sol::state& lua); //this is needed
    void RegisterECS(sol::state& lua);
    void RegisterGUIWindow(sol::state& lua, View::GUIWindow* gui);
    void RegisterRenderer(sol::state& lua, const std::shared_ptr<View::Renderer>& renderer);
    void RegisterPhysics(sol::state& lua);
    void RegisterScene(sol::state& lua);
    void RegisterMath(sol::state& lua);
    void RegisterOther(sol::state& lua);
}  // namespace Vakol::Controller