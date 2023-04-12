#include "LuaAccess.hpp"

#include "AssetLoader/AssetLoader.hpp"
#include "Model/Components.hpp"

namespace Vakol::Controller {
    void RegisterMath(sol::state& lua) {
        auto vec3 = lua.new_usertype<glm::vec3>("vec3");
        vec3["x"] = &glm::vec3::x;
        vec3["y"] = &glm::vec3::y;
        vec3["z"] = &glm::vec3::z;
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

    void RegisterAssetLoader(sol::state& lua) {
        lua.set_function("load_texture", [](std::string path) {
            auto tex = AssetLoader::GetTexture(path);
            if (tex == nullptr) return false;

            return true;
        });

        lua.set_function("load_model", [](std::string path) {
            auto model = AssetLoader::GetModel(path);
            if (model == nullptr) return false;

            return true;
        });

        lua.set_function("load_shader", [](std::string path) {
            auto shader = AssetLoader::GetShader(path);
            if (shader == nullptr) return false;

            return true;
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
    void RegisterEntity(sol::state& lua) {
        auto entityType = lua.new_usertype<Entity>("entity");

        entityType.set_function("get_transform", &Entity::GetComponent<Model::Components::Transform>);
        entityType.set_function("add_model", [](Entity* ent, std::string path) {
            if (ent->HasComponent<Model::Components::Drawable>() == false) {
                ent->AddComponent<Model::Components::Drawable>();
            }
            auto model = AssetLoader::GetModel(path);
            if (model == nullptr) return false;

            ent->GetComponent<Model::Components::Drawable>().model_ptr = model;

            return true;
        });
    }

    void RegisterECS(sol::state& lua) {
        auto TransformType = lua.new_usertype<Model::Components::Transform>("transform");

        TransformType["pos"] = &Model::Components::Transform::pos;
        TransformType["rot"] = &Model::Components::Transform::rot;
        TransformType["scale"] = &Model::Components::Transform::scale;
    }

    void RegisterScene(sol::state& lua) {
        auto sceneType = lua.new_usertype<Scene>("scene");

        sceneType.set_function("create_entity", &Scene::CreateEntity);
    }

    void RegisterWindow(sol::state& lua) {}
    void RegisterRenderer(sol::state& lua) {}
    void RegisterPhysics(sol::state& lua) {}
}  // namespace Vakol::Controller
