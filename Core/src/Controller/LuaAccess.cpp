#include "LuaAccess.hpp"

#include "Logger.hpp"

namespace Vakol::Controller {
    void RegisterGameConfig(sol::state& lua) {
        // auto gameConfigType = lua.new_usertype<GameConfig>("game_config");

        // gameConfigType["name"] = &GameConfig::name;
        // gameConfigType["window"]["w"] = &GameConfig::windowWidth;
        // gameConfigType["window"]["h"] = &GameConfig::windowHeight;
        // gameConfigType["renderer"] = &GameConfig::rendererType;
    }

    void RegisterLogger(sol::state& lua) {
        lua.set_function("print", &Logger::ScriptPrintTrace);
        lua.set_function("print_info", &Logger::ScriptPrintInfo);
        lua.set_function("print_warn", &Logger::ScriptPrintWarn);
        lua.set_function("print_err", &Logger::ScriptPrintError);
        lua.set_function("print_crit", &Logger::ScriptPrintCrit);
    }
}  // namespace Vakol::Controller
