#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Vakol
{
    /**
     * @brief Wrapper around sol environment to have a reference to an active script
     *
     */
    struct LuaScript
    {
        sol::environment env;
        std::string path;
    };

    using LuaType = sol::object;
    using LuaTable = sol::table;
    using LuaFunction = sol::protected_function;

    using LuaState = sol::state;
} // namespace Vakol