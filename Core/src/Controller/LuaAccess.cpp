#include "LuaAccess.hpp"

#include "Logger.hpp"
#include "Scene.hpp"

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

    void RegisterApplication(sol::state& lua) {}

    void RegisterScene(sol::state& lua) {
        // auto
        lua.set_function("load_script", &Scene::LoadScript);
    }
    void RegisterECS(sol::state& lua) {}
    void RegisterWindow(sol::state& lua) {}
    void RegisterRenderer(sol::state& lua) {}
    void RegisterPhysics(sol::state& lua) {}
}  // namespace Vakol::Controller
