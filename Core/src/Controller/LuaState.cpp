#include "LuaState.hpp"

namespace Vakol::Controller {
    LuaState::LuaState() { lua.open_libraries(sol::lib::base); }

    const sol::state& LuaState::GetState() const { return lua; }

    sol::state& LuaState::GetState() { return lua; }

    void LuaState::RunFile(const std::string& file) { lua.script_file(file); }

    void LuaState::RunScript(const std::string& script) { lua.script(script); }

}  // namespace Vakol::Controller