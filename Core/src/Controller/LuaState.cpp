#include "LuaState.hpp"

namespace Vakol::Controller {
    LuaState::LuaState() {
        lua.open_libraries(sol::lib::base);
        lua.open_libraries(sol::lib::math);
    }

    const sol::state& LuaState::GetState() const { return lua; }

    sol::state& LuaState::GetState() { return lua; }

    sol::protected_function_result LuaState::RunFile(const std::string& file) { return lua.safe_script_file(file); }

    void LuaState::RunScript(const std::string& script) { lua.safe_script(script); }

}  // namespace Vakol::Controller