#pragma once
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace Vakol::Controller {
    class LuaState {
       public:
        LuaState();

        const sol::state& GetState() const;
        sol::state& GetState();

        sol::protected_function_result RunFile(const std::string& file);

        void RunFunction(std::string funcName);
        void RunFunction(sol::function luaFunction);

       private:
        sol::state lua;
    };
}  // namespace Vakol::Controller