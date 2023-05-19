#include "LuaState.hpp"

#include <iostream>
#include <sstream>

#include "Logger.hpp"

std::string getStatusString(const sol::call_status status)
{
    switch (status) {
        case sol::call_status::ok:
            return "ok";
        case sol::call_status::yielded:
            return "yielded";
        case sol::call_status::runtime:
            return "runtime";
        case sol::call_status::memory:
            return "memory";
        case sol::call_status::handler:
            return "handler";
        case sol::call_status::gc:
            return "gc";
        case sol::call_status::syntax:
            return "syntax";
        case sol::call_status::file:
            return "file";
        default:
            return "unknown";
    }
}

namespace Vakol::Controller {
    LuaState::LuaState() {
        lua.open_libraries(sol::lib::base);
        lua.open_libraries(sol::lib::math);
        lua.open_libraries(sol::lib::string);
    }

    const sol::state& LuaState::GetState() const { return lua; }

    sol::state& LuaState::GetState() { return lua; }

    sol::protected_function_result LuaState::RunFile(const std::string& file) {
        auto result = lua.safe_script_file(file, &sol::script_pass_on_error);

        if (!result.valid()) {
	        const sol::error err = result;
            const sol::call_status status = result.status();
            std::ostringstream oss;
            oss << "Lua script execution error: " << err.what() << " (status: " << getStatusString(status) << ")";
	        const std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
        }

        return result;
    }

    void LuaState::RunFunction(const std::string& funcName) {
        sol::function func = lua[funcName];

        if (!func.valid()) {
            std::ostringstream oss;
            oss << "Lua script execution error: Function '" << funcName << "' not found";
            std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
            return;
        }

        if (sol::protected_function_result result = func(); !result.valid()) 
        {
            sol::error err = result;
            sol::call_status status = result.status();

            std::ostringstream oss;
            oss << "Lua script execution error: " << err.what() << " (status: " << getStatusString(status) << ")";
            std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
        }
    }

    void LuaState::RunFunction(const sol::function& luaFunction)
	{
        if (!luaFunction.valid()) {
            std::ostringstream oss;
            oss << "Lua script execution error: Invalid function";
            std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
            return;
        }

        if (sol::protected_function_result result = luaFunction(); !result.valid()) {
            sol::error err = result;
            sol::call_status status = result.status();

            std::ostringstream oss;
            oss << "Lua script execution error: " << err.what() << " (status: " << getStatusString(status) << ")";
            std::string errorMsg = oss.str();

            VK_ERROR(errorMsg);
        }
    }
}