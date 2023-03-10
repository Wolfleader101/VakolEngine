#pragma once

#include <entt/entt.hpp>

#include "LuaState.hpp"

namespace Vakol::Controller::System {

    using reg = entt::registry;

    // -- READ --
    // if you add a function use the convention Type_Action()
    // will make it more intuitive to find names of funcs

    void Model_Draw(reg& registry);

    void ScriptUpdate(reg& registry, LuaState& lua);

}  // namespace Vakol::Controller::System