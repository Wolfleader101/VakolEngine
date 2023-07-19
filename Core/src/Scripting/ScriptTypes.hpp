#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Vakol {
    // prefixing with Lua for now to avoid name collisions
    struct LuaScript {
        sol::environment env;
        std::string path;
    };

    using LuaType = sol::object;
    using LuaTable = sol::table;
    using LuaFunction = sol::protected_function;

    //! this is not good, we shouldnt want to expose sol::state, but we need it for now because of FSM

    //! update, i feel like maybe we should expose it?
    using LuaState = sol::state;
}  // namespace Vakol