#include "LuaAccess.hpp"

namespace Vakol::Controller {
    void RegisterGameConfig(sol::state& lua) {
        // auto gameConfigType = lua.new_usertype<GameConfig>("game_config");

        // gameConfigType["name"] = &GameConfig::name;
        // gameConfigType["window"]["w"] = &GameConfig::windowWidth;
        // gameConfigType["window"]["h"] = &GameConfig::windowHeight;
        // gameConfigType["renderer"] = &GameConfig::rendererType;
    }

    void RegisterLogger(sol::state& lua) {
        lua.set_function("print", [](sol::variadic_args va) {
            auto arg = va[0];

            if (arg.get_type() == sol::type::string) {
                Logger::ScriptPrintTrace(va[0].get<std::string>());
            } else if (arg.get_type() == sol::type::number) {
                Logger::ScriptPrintTrace(std::to_string(va[0].get<float>()));
            }
        });
        lua.set_function("print_info", [](sol::variadic_args va) {
            auto arg = va[0];

            if (arg.get_type() == sol::type::string) {
                Logger::ScriptPrintInfo(va[0].get<std::string>());
            } else if (arg.get_type() == sol::type::number) {
                Logger::ScriptPrintInfo(std::to_string(va[0].get<float>()));
            }
        });
        lua.set_function("print_warn", [](sol::variadic_args va) {
            auto arg = va[0];

            if (arg.get_type() == sol::type::string) {
                Logger::ScriptPrintWarn(va[0].get<std::string>());
            } else if (arg.get_type() == sol::type::number) {
                Logger::ScriptPrintWarn(std::to_string(va[0].get<float>()));
            }
        });
        lua.set_function("print_err", [](sol::variadic_args va) {
            auto arg = va[0];

            if (arg.get_type() == sol::type::string) {
                Logger::ScriptPrintError(va[0].get<std::string>());
            } else if (arg.get_type() == sol::type::number) {
                Logger::ScriptPrintError(std::to_string(va[0].get<float>()));
            }
        });
        lua.set_function("print_crit", [](sol::variadic_args va) {
            auto arg = va[0];

            if (arg.get_type() == sol::type::string) {
                Logger::ScriptPrintCrit(va[0].get<std::string>());
            } else if (arg.get_type() == sol::type::number) {
                Logger::ScriptPrintCrit(std::to_string(va[0].get<float>()));
            }
        });
    }

    void RegisterApplication(sol::state& lua, Application* app) {
        lua.set_function("add_scene", &Application::AddScene, app);

        auto TimeType = lua.new_usertype<Time>("Time");
        TimeType["delta_time"] = &Time::deltaTime;
        TimeType["curr_time"] = &Time::curTime;
        TimeType["prev_time"] = &Time::prevTime;

        lua["Time"] = &app->GetTime();
    }
    void RegisterECS(sol::state& lua) {}
    void RegisterWindow(sol::state& lua) {}
    void RegisterRenderer(sol::state& lua) {}
    void RegisterPhysics(sol::state& lua) {}

    void RegisterScene(sol::state& lua) {
        auto sceneType = lua.new_usertype<Scene>("scene");

        sceneType.set_function("create_entity", &Scene::CreateEntity);
    }
}  // namespace Vakol::Controller
