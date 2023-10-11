#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <vector>

namespace Vakol
{
    using LuaEnvironment = sol::environment;

    using LuaType = sol::object;
    using LuaTable = sol::table;
    using LuaFunction = sol::protected_function;

    using LuaState = sol::state;
    /**
     * @brief Wrapper around sol environment to have a reference to an active script
     *
     */
    struct LuaScript
    {
        std::string name = "";
        LuaEnvironment env = {};
        std::string path = "";
    };

    /**
     * @brief script compoenent for an entity
     *
     */
    struct ScriptComp
    {
        std::vector<LuaScript> scripts;
    };

} // namespace Vakol