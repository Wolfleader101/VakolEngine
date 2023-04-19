#include "LuaAccess.hpp"

#include "AssetLoader/AssetLoader.hpp"
#include "Model/Assets/Material.hpp"
#include "Model/Components.hpp"
#include "Model/gl/GLInstance.hpp"
#include "View/GUI/GUIWindow.hpp"

using Vakol::Model::Assets::Material;

const int DIRECTIONAL_LIGHT = 0;
const int POINT_LIGHT = 1;
const int SPOT_LIGHT = 2;

namespace Vakol::Controller {
    void RegisterMath(sol::state& lua) {
        auto vec3 = lua.new_usertype<glm::vec3>("vec3");
        vec3["x"] = &glm::vec3::x;
        vec3["y"] = &glm::vec3::y;
        vec3["z"] = &glm::vec3::z;

        auto vec2 = lua.new_usertype<glm::vec2>("vec2");
        vec2["x"] = &glm::vec2::x;
        vec2["y"] = &glm::vec2::y;
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
            // auto tex = AssetLoader::GetTexture(path);
            // if (tex == nullptr) return false;

            // return true;
        });

        lua.set_function("load_model", [](std::string path) {
            auto model = AssetLoader::GetModel(path);
            if (model == nullptr) return false;

            return true;
        });

        lua.set_function("load_shader", [](std::string path) {
            // //auto shader = AssetLoader::GetShader(path);
            // if (shader == nullptr) return false;

            // return true;
        });
    }

    void RegisterApplication(sol::state& lua, Application* app) {
        lua.set_function("add_scene", &Application::AddScene, app);

        auto TimeType = lua.new_usertype<Time>("Time");
        TimeType["delta_time"] = &Time::deltaTime;
        TimeType["curr_time"] = &Time::curTime;
        TimeType["prev_time"] = &Time::prevTime;

        lua["Time"] = &app->GetTime();

        auto InputType = lua.new_usertype<Input>("Input");
        InputType.set_function("get_key", &Input::GetKey);
        InputType.set_function("get_key_down", &Input::GetKeyDown);
        InputType.set_function("get_key_up", &Input::GetKeyUp);
        InputType.set_function("get_mouse_pos", &Input::GetMousePos);
        InputType.set_function("get_delta_mouse_pos", &Input::GetDeltaMousePos);

        lua["Input"] = &app->GetInput();

        lua["KEYS"] = lua.create_table_with(
            "KEY_SPACE", Input::KEY::KEY_SPACE, "KEY_APOSTROPHE", Input::KEY::KEY_APOSTROPHE, "KEY_COMMA",
            Input::KEY::KEY_COMMA, "KEY_MINUS", Input::KEY::KEY_MINUS, "KEY_PERIOD", Input::KEY::KEY_PERIOD,
            "KEY_SLASH", Input::KEY::KEY_SLASH, "KEY_0", Input::KEY::KEY_0, "KEY_1", Input::KEY::KEY_1, "KEY_2",
            Input::KEY::KEY_2, "KEY_3", Input::KEY::KEY_3, "KEY_4", Input::KEY::KEY_4, "KEY_5", Input::KEY::KEY_5,
            "KEY_6", Input::KEY::KEY_6, "KEY_7", Input::KEY::KEY_7, "KEY_8", Input::KEY::KEY_8, "KEY_9",
            Input::KEY::KEY_9, "KEY_SEMICOLON", Input::KEY::KEY_SEMICOLON, "KEY_EQUAL", Input::KEY::KEY_EQUAL, "KEY_A",
            Input::KEY::KEY_A, "KEY_B", Input::KEY::KEY_B, "KEY_C", Input::KEY::KEY_C, "KEY_D", Input::KEY::KEY_D,
            "KEY_E", Input::KEY::KEY_E, "KEY_F", Input::KEY::KEY_F, "KEY_G", Input::KEY::KEY_G, "KEY_H",
            Input::KEY::KEY_H, "KEY_I", Input::KEY::KEY_I, "KEY_J", Input::KEY::KEY_J, "KEY_K", Input::KEY::KEY_K,
            "KEY_L", Input::KEY::KEY_L, "KEY_M", Input::KEY::KEY_M, "KEY_N", Input::KEY::KEY_N, "KEY_O",
            Input::KEY::KEY_O, "KEY_P", Input::KEY::KEY_P, "KEY_Q", Input::KEY::KEY_Q, "KEY_R", Input::KEY::KEY_R,
            "KEY_S", Input::KEY::KEY_S, "KEY_T", Input::KEY::KEY_T, "KEY_U", Input::KEY::KEY_U, "KEY_V",
            Input::KEY::KEY_V, "KEY_W", Input::KEY::KEY_W, "KEY_X", Input::KEY::KEY_X, "KEY_Y", Input::KEY::KEY_Y,
            "KEY_Z", Input::KEY::KEY_Z, "KEY_LEFT_SHIFT", Input::KEY::KEY_LEFT_SHIFT);
    }

    void RegisterEntity(sol::state& lua) {
        auto entityType = lua.new_usertype<Entity>("entity");

        entityType.set_function("get_transform", &Entity::GetComponent<Model::Components::Transform>);
        entityType.set_function("add_model", [](Entity* ent, std::string path) {
            if (ent->HasComponent<Model::Components::Drawable>() == false)
                ent->AddComponent<Model::Components::Drawable>();

            auto model = AssetLoader::GetModel(path);

            if (model == nullptr) return false;

            auto material = model->meshes().begin()->material();

            for (const auto& texture : material->textures()) {
                GLTexture tex("coreAssets/textures/" + texture.path);
                tex.Bind();
            }

            // force it for now, since I don't understand lua lol
            material->SetShader("coreAssets/shaders/basic.prog");
            material->Bind();

            // material->SetInt("texture_0", 0);

            // material->SetInt("material.diffuse", 0);
            // material->SetInt("material.specular", 1);
            // material->SetInt("material.emissive", 2);

            // material->SetFloat("material.shininess", 64.0f);

            // material->SetVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            // material->SetVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            // material->SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

            // material->SetFloat("light.constant", 1.0f);
            // material->SetFloat("light.linear", 0.045f);
            // material->SetFloat("light.quadratic", 0.0075f);
            // material->SetFloat("light.cut_off", glm::cos(glm::radians(12.5f)));
            // material->SetFloat("light.outer_cut_off", glm::cos(glm::radians(17.5f)));

            // material->SetInt("option", SPOT_LIGHT);
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
        auto cameraType = lua.new_usertype<Camera>("camera");

        sceneType.set_function("create_entity", &Scene::CreateEntity);
        sceneType.set_function("get_camera", &Scene::GetCamera);

        cameraType.set_function("get_pos", &Camera::GetPos);
        cameraType.set_function("set_pos", &Camera::SetPos);
        cameraType.set_function("get_forward", &Camera::GetForward);
        cameraType.set_function("get_right", &Camera::GetRight);

        cameraType.set_function("get_pitch", &Camera::GetPitch);
        cameraType.set_function("set_pitch", &Camera::SetPitch);

        cameraType.set_function("get_yaw", &Camera::GetYaw);
        cameraType.set_function("set_yaw", &Camera::SetYaw);
    }

    void RegisterGUIWindow(sol::state& lua, View::GUIWindow* gui) {
        auto guiWindowType =
            lua.new_usertype<View::GUIWindow>("gui_window");  // Creates a new usertype of the type 'View::GUIWindow'

        lua["GUI"] = gui;

        // REGISTERS C++ FUNCTIONS TO LUA
        guiWindowType.set_function("start_window_creation", &View::GUIWindow::StartWindowCreation);

        guiWindowType.set_function("get_fps", &View::GUIWindow::GetFramesPerSecond);

        guiWindowType.set_function("add_text", &View::GUIWindow::AddText);
        guiWindowType.set_function("add_button", &View::GUIWindow::AddButton);
        guiWindowType.set_function("add_checkbox", &View::GUIWindow::AddCheckbox);

        guiWindowType.set_function("add_integer_slider", &View::GUIWindow::AddIntSlider);
        guiWindowType.set_function("add_float_slider", &View::GUIWindow::AddFloatSlider);

        guiWindowType.set_function("add_vector_integer_slider", &View::GUIWindow::AddVecIntSlider);
        guiWindowType.set_function("add_vector_float_slider", &View::GUIWindow::AddVecFloatSlider);

        guiWindowType.set_function("end_window_creation", &View::GUIWindow::EndWindowCreation);
    }

    void RegisterRenderer(sol::state& lua) {}
    void RegisterPhysics(sol::state& lua) {}
}  // namespace Vakol::Controller
