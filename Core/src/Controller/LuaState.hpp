#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Vakol::Controller {
    /**
     * @class LuaState
     *
     * @brief Class representing the Lua state.
     */
    class LuaState {
       public:
        /**
         * @brief Construct a new LuaState object.
         */
        LuaState();

        /**
         * @brief Get a const reference to the Lua state.
         *
         * @return const sol::state& The const reference to the Lua state.
         */
        [[nodiscard]] const sol::state& GetState() const;

        /**
         * @brief Get a reference to the Lua state.
         *
         * @return sol::state& The reference to the Lua state.
         */
        sol::state& GetState();

        /**
         * @brief Run a Lua script file.
         *
         * @param file The file path of the Lua script.
         * @return sol::protected_function_result The result of running the Lua script.
         */
        sol::protected_function_result RunFile(const std::string& file);

        /**
         * @brief Run a Lua function by its name.
         *
         * @param funcName The name of the Lua function.
         */
        void RunFunction(const std::string& funcName);

        /**
         * @brief Run a Lua function.
         *
         * @param luaFunction The Lua function to run.
         */
        void RunFunction(const sol::function& luaFunction);

       private:
        /**
         * @brief The Lua state.
         */
        sol::state lua;
    };
}  // namespace Vakol::Controller