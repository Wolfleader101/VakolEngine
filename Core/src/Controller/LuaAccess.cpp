#include "LuaAccess.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "AssetLoader/AssetLoader.hpp"
#include "AssetLoader/TextureLoader.hpp"
#include "Model/Assets/Material.hpp"
#include "Model/Components.hpp"
#include "Model/Instance.hpp"
#include "System.hpp"
#include "View/GUI/GUIWindow.hpp"

namespace Vakol::Controller {
    void RegisterMath(sol::state& lua) {
        {
            sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)> ctor;  // allow for constructors

            auto div_overload = sol::overload([](const glm::vec2& v, const float k) -> glm::vec2 { return v / k; },
                                              [](const float k, const glm::vec2& v) -> glm::vec2 { return k / v; });

            auto mul_overload = sol::overload([](const glm::vec2& u, const glm::vec2& v) -> glm::vec2 { return u * v; },
                                              [](const glm::vec2& v, const float k) -> glm::vec2 { return v * k; },
                                              [](const float k, const glm::vec2& v) -> glm::vec2 { return k * v; });

            auto add_overload =
                sol::overload([](const glm::vec2& u, const glm::vec2& v) -> glm::vec2 { return u + v; });

            auto sub_overload =
                sol::overload([](const glm::vec2& u, const glm::vec2& v) -> glm::vec2 { return u - v; });

            auto vec2 =
                lua.new_usertype<glm::vec2>("Vector2", ctor, sol::meta_function::multiplication, mul_overload,
                                            sol::meta_function::division, div_overload, sol::meta_function::addition,
                                            add_overload, sol::meta_function::subtraction, sub_overload);

            vec2["x"] = &glm::vec2::x;
            vec2["y"] = &glm::vec2::y;

            vec2["r"] = &glm::vec2::r;
            vec2["g"] = &glm::vec2::g;
        }

        {
            sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>
                ctor;  // allow for constructors

            auto div_overload = sol::overload([](const glm::vec3& v, const float k) -> glm::vec3 { return v / k; },
                                              [](const float k, const glm::vec3& v) -> glm::vec3 { return k / v; });

            auto mul_overload = sol::overload([](const glm::vec3& u, const glm::vec3& v) -> glm::vec3 { return u * v; },
                                              [](const glm::vec3& v, const float k) -> glm::vec3 { return v * k; },
                                              [](const float k, const glm::vec3& v) -> glm::vec3 { return k * v; });

            auto add_overload =
                sol::overload([](const glm::vec3& u, const glm::vec3& v) -> glm::vec3 { return u + v; });

            auto sub_overload =
                sol::overload([](const glm::vec3& u, const glm::vec3& v) -> glm::vec3 { return u - v; });

            auto vec3 =
                lua.new_usertype<glm::vec3>("Vector3", ctor, sol::meta_function::multiplication, mul_overload,
                                            sol::meta_function::division, div_overload, sol::meta_function::addition,
                                            add_overload, sol::meta_function::subtraction, sub_overload);

            vec3["x"] = &glm::vec3::x;
            vec3["y"] = &glm::vec3::y;
            vec3["z"] = &glm::vec3::z;

            vec3["r"] = &glm::vec3::r;
            vec3["g"] = &glm::vec3::g;
            vec3["b"] = &glm::vec3::b;

            vec3.set_function("magnitude", [](const glm::vec3& v) -> float { return glm::length(v); });
        }

        {
            sol::constructors<glm::vec4(), glm::vec4(float), glm::vec4(float, float, float, float)>
                ctor;  // allow for constructors

            auto div_overload = sol::overload([](const glm::vec4& v, const float k) -> glm::vec4 { return v / k; },
                                              [](const float k, const glm::vec4& v) -> glm::vec4 { return k / v; });

            auto mul_overload =
                sol::overload([](const glm::vec4& u, const glm::vec4& v) -> glm::vec4 { return u * v; },
                              [](const glm::vec4& v, const float k) -> glm::vec4 { return v * k; },
                              [](const float k, const glm::vec4& v) -> glm::vec4 { return k * v; },
                              [](const glm::mat4& m, const glm::vec4& v) -> glm::vec4 { return m * v; },
                              [](const glm::vec4& v, const glm::mat4& m) -> glm::vec4 { return v * m; });

            auto add_overload =
                sol::overload([](const glm::vec4& u, const glm::vec4& v) -> glm::vec4 { return u + v; });

            auto sub_overload =
                sol::overload([](const glm::vec4& u, const glm::vec4& v) -> glm::vec4 { return u - v; });

            auto vec4 =
                lua.new_usertype<glm::vec4>("Vector4", ctor, sol::meta_function::multiplication, mul_overload,
                                            sol::meta_function::division, div_overload, sol::meta_function::addition,
                                            add_overload, sol::meta_function::subtraction, sub_overload);

            vec4["x"] = &glm::vec4::x;
            vec4["y"] = &glm::vec4::y;
            vec4["z"] = &glm::vec4::z;
            vec4["w"] = &glm::vec4::w;

            vec4["r"] = &glm::vec4::r;
            vec4["g"] = &glm::vec4::g;
            vec4["b"] = &glm::vec4::b;
            vec4["a"] = &glm::vec4::a;
        }

        {
            sol::constructors<glm::mat4(), glm::mat4(float), glm::mat4(glm::vec4, glm::vec4, glm::vec4, glm::vec4)>
                ctor;

            auto mul_overload =
                sol::overload([](const glm::mat4& lhs, const glm::mat4& rhs) -> glm::mat4 { return lhs * rhs; },
                              [](const glm::mat4& m, const float k) -> glm::mat4 { return m * k; },
                              [](const float k, const glm::mat4& m) -> glm::mat4 { return k * m; });

            auto mat4 =
                lua.new_usertype<glm::mat4>("Matrix4x4", ctor, sol::meta_function::multiplication, mul_overload);

            lua.set_function("translate", [](const glm::mat4& matrix, const glm::vec3& translation) {
                return translate(matrix, translation);
            });

            lua.set_function("scale",
                             [](const glm::mat4& matrix, const glm::vec3& scale) { return glm::scale(matrix, scale); });

            lua.set_function("rotate", [](const glm::mat4& matrix, const float angle, const glm::vec3& axis) {
                return rotate(matrix, angle, axis);
            });

            lua.set_function("inverse", [](const glm::mat4& matrix) { return inverse(matrix); });

            lua.set_function("transpose", [](const glm::mat4& matrix) { return transpose(matrix); });
        }
    }

    void RegisterLogger(sol::state& lua) {
        lua.set_function("print", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintTrace(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintTrace(std::to_string(va[0].get<float>()));
        });
        lua.set_function("print_info", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintInfo(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintInfo(std::to_string(va[0].get<float>()));
        });
        lua.set_function("print_warn", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintWarn(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintWarn(std::to_string(va[0].get<float>()));
        });
        lua.set_function("print_err", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintError(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintError(std::to_string(va[0].get<float>()));
        });
        lua.set_function("print_critical", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintCrit(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintCrit(std::to_string(va[0].get<float>()));
        });
    }

    void RegisterAssetLoader(sol::state& lua) {
        lua.set_function("load_texture", [](std::string& path) {
            return AssetLoader::GetTexture(path);  // no checks... just raw doggin it LOL
        });

        lua.set_function("load_model",
                         [](const std::string& path, const float scale = 1.0f, const bool animated = false) {
                             const auto model = AssetLoader::GetModel(path, scale, animated);

                             if (model == nullptr) return false;

                             return true;
                         });

        lua.set_function("load_shader", [](const std::string& path) {
            const auto shader = AssetLoader::GetShader(path);

            if (shader == nullptr) return false;

            return true;
        });
    }

    void RegisterApplication(sol::state& lua, Application* app) {
        lua.set_function("add_scene", &Application::AddScene, app);

        auto time_type = lua.new_usertype<Time>("Time");
        time_type["delta_time"] = &Time::deltaTime;
        time_type["curr_time"] = &Time::curTime;
        time_type["prev_time"] = &Time::prevTime;
        time_type["fps"] = &Time::fps;

        lua["Time"] = &app->GetTime();

        auto input_type = lua.new_usertype<Input>("Input");
        input_type.set_function("get_key", &Input::GetKey);
        input_type.set_function("get_key_down", &Input::GetKeyDown);
        input_type.set_function("get_key_up", &Input::GetKeyUp);
        input_type.set_function("get_mouse_pos", &Input::GetMousePos);
        input_type.set_function("get_delta_mouse_pos", &Input::GetDeltaMousePos);

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

    void RegisterEntity(LuaState& state, sol::state& lua) {
        auto entity_type = lua.new_usertype<Entity>("entity");
        auto model_type = lua.new_usertype<Assets::Model>("model");
        auto mesh_type = lua.new_usertype<Mesh>("mesh");
        auto material_type = lua.new_usertype<Assets::Material>("material");
        auto shader_type = lua.new_usertype<Shader>("shader");

        lua.set_function("create_raw_texture", [](std::string& path) {
            auto texture = Assets::Texture(path);
            texture.SetID(LoadRawTexture(texture.path));

            return texture;
        });

        lua.set_function("create_texture", [](std::string& path, const bool gamma, const bool flip) {
            auto texture = Assets::Texture(path);
            texture.SetID(LoadTexture(texture.path, gamma, flip));

            return texture;
        });

        entity_type.set_function("get_transform", &Entity::GetComponent<Transform>);

        entity_type.set_function("create_height_map_terrain", [](Entity* ent, std::string&& path, const float min, const float max)
        {
            if (!ent->HasComponent<Drawable>()) ent->AddComponent<Drawable>();

		     if (ent->HasComponent<Terrain>()) ent->RemoveComponent<Terrain>();

		     ent->AddComponent<Terrain>(LoadHeightMapTerrain(std::move(path), min, max));

		     auto terrain = ent->GetComponent<Terrain>();

		     if (const auto model = terrain.GetModel()) {
		         model->mesh().SetDrawMode(DRAW_MODE::STRIPS);
		         model->mesh().SetDrawType(DRAW_TYPE::ELEMENTS);

		         model->mesh().SetDrawModeInfo((terrain.GetSize() - 1) / 1);  // num strips

		         model->mesh().SetNumTrisPerStrip(terrain.GetSize() / 1 * 2 - 2);

		         ent->GetComponent<Components::Drawable>().model_ptr = model;
		     }

		     return terrain;
		});

        entity_type.set_function("create_clod_terrain", [](Entity* ent, std::string&& path) 
        {
            if (!ent->HasComponent<Drawable>()) ent->AddComponent<Drawable>();

            if (ent->HasComponent<Terrain>()) ent->RemoveComponent<Terrain>();

            ent->AddComponent<Terrain>(LoadCLODTerrain(std::move(path)));

            auto terrain = ent->GetComponent<Terrain>();

            if (const auto model = terrain.GetModel()) {
                model->mesh().SetDrawMode(DRAW_MODE::PATCHES);
                model->mesh().SetDrawType(DRAW_TYPE::ARRAYS);

                model->mesh().SetDrawModeInfo(400);  // num patches

                ent->GetComponent<Drawable>().model_ptr = model;
            }

            return terrain;
        });

        entity_type.set_function("get_terrain", [](const Entity* ent) {
            if (ent->HasComponent<Terrain>()) return ent->GetComponent<Terrain>();
        });

        entity_type.set_function("add_model", [](Entity* ent, const std::string& path, const float scale = 1.0f,
                                                 const bool animated = false) {
            if (!ent->HasComponent<Drawable>()) ent->AddComponent<Drawable>();

            auto model = AssetLoader::GetModel(path, scale, animated);

            if (model) ent->GetComponent<Drawable>().model_ptr = model;

            return model;
        });

        entity_type.set_function("instantiate_model",
                                 [](const std::shared_ptr<Assets::Model>& model,
                                    const sol::as_table_t<std::vector<glm::mat4>>& matrices, int amount) {
                                     VK_TRACE(matrices.value().size());
                                     CreateInstances(model->meshes(), matrices.value());
                                 });

        entity_type.set_function("get_model", [](const Entity* ent) {
            if (ent->HasComponent<Drawable>()) return ent->GetComponent<Drawable>().model_ptr;
        });

        model_type.set_function("set_animation_state", &Assets::Model::SetAnimationState);

        model_type.set_function("get_mesh_count", &Assets::Model::nMeshes);
        model_type.set_function("get_mesh", &Assets::Model::mesh);

        model_type.set_function("set_shader", &Assets::Model::set_shader);
        model_type.set_function("get_shader", &Assets::Model::shader);

        mesh_type.set_function("get_material", &Mesh::GetMaterial);

        material_type.set_function("add_texture", &Assets::Material::AddTexture);
        material_type.set_function("get_texture", &Assets::Material::GetTexture);
        material_type.set_function("get_ambient_color", &Assets::Material::GetAmbientColor);
        material_type.set_function("get_diffuse_color", &Assets::Material::GetDiffuseColor);

        shader_type.set_function("set_int", &Shader::SetInt);
        shader_type.set_function("set_float", &Shader::SetFloat);

        shader_type.set_function("set_vec2v",
                                 sol::resolve<void(const char*, const glm::vec2&) const>(&Shader::SetVec2));
        shader_type.set_function("set_vec3v",
                                 sol::resolve<void(const char*, const glm::vec3&) const>(&Shader::SetVec3));
        shader_type.set_function("set_vec4v",
                                 sol::resolve<void(const char*, const glm::vec4&) const>(&Shader::SetVec4));

        shader_type.set_function("set_vec2",
                                 sol::resolve<void(const char*, float, float) const>(&Shader::SetVec2));
        shader_type.set_function(
            "set_vec3", sol::resolve<void(const char*, float, float, float) const>(&Shader::SetVec3));
        shader_type.set_function(
            "set_vec4", sol::resolve<void(const char*, float, float, float, float) const>(
                            &Shader::SetVec4));

        entity_type.set_function("physics_init", [](Entity* ent, Scene& scene) { System::BindScene(scene); });

        entity_type.set_function("add_rigid", [](Entity* ent) -> RigidBody& {
            if (!ent->HasComponent<RigidBody>()) ent->AddComponent<RigidBody>();
            return ent->GetComponent<RigidBody>();
        });

        entity_type.set_function("get_rigid", [](const Entity* ent) -> RigidBody& {
            if (ent->HasComponent<RigidBody>()) return ent->GetComponent<RigidBody>();

            VK_CRITICAL("No rigid body component found on entity");
            assert(0);
        });

        entity_type.set_function("add_collider", [](Entity* ent) -> Collider& {
            if (!ent->HasComponent<Collider>()) ent->AddComponent<Collider>();
            return ent->GetComponent<Collider>();
        });

        entity_type.set_function("get_collider", [](const Entity* ent) -> Collider& {
            if (ent->HasComponent<Collider>()) return ent->GetComponent<Collider>();

            VK_CRITICAL("No collider component found on entity");
            assert(0);
        });

        entity_type.set_function("get_bounds_from_model", [](Entity* ent) -> void {
            if (ent->HasComponent<Drawable, Collider>()) {
                const auto& model = ent->GetComponent<Drawable>();

                auto& collider = ent->GetComponent<Collider>();

                collider.bounds = GetBounds(model);
            } else {
                VK_CRITICAL("drawable and collider must be present to get bounds from");
                assert(0);
            }
        });

        entity_type.set_function("get_bounds_from_model", [](Entity* ent) -> void {
            if (ent->HasComponent<Drawable, Collider>()) {
                const auto& model = ent->GetComponent<Drawable>();

                auto& collider = ent->GetComponent<Components::Collider>();

                collider.bounds = GetBounds(model);
            } else {
                VK_CRITICAL("drawable and collider must be present to get bounds from");
                assert(0);
            }
        });

        entity_type.set_function("add_fsm", [&](Entity* ent) -> Components::FSM& {
            if (!ent->HasComponent<FSM>()) ent->AddComponent<FSM>(state);
            return ent->GetComponent<FSM>();
        });
    }

    void RegisterECS(sol::state& lua) {
        auto transform_type = lua.new_usertype<Transform>("transform");

        transform_type["pos"] = &Transform::pos;
        transform_type["rot"] = &Transform::rot;
        transform_type["scale"] = &Transform::scale;

        auto terrain_type = lua.new_usertype<Terrain>("terrain");
        // terrainType.set_function("load_heightmap", &Terrain::LoadHeightMap);
        // terrainType.set_function("load_texture", &Terrain::LoadTexture);
        // terrainType.set_function("generate", &Terrain::Generate);

        terrain_type.set_function("get_height", &Terrain::GetHeight);
        terrain_type.set_function("get_size", &Terrain::GetSize);
        terrain_type.set_function("get_model", &Terrain::GetModel);

        auto fsm_type = lua.new_usertype<FSM>("FSM");
        fsm_type["get_state"] = &FSM::GetState;
        fsm_type["change_state"] = &FSM::ChangeState;
        fsm_type["add_state"] = &FSM::AddState;
        fsm_type["update"] = &FSM::Update;
    }

    void RegisterScene(sol::state& lua) {
        auto scene_type = lua.new_usertype<Scene>("scene");
        auto camera_type = lua.new_usertype<Camera>("camera");

        scene_type["globals"] = &Scene::sceneGlobals;

        scene_type.set_function("create_entity", &Scene::CreateEntity);
        scene_type.set_function("get_camera", &Scene::GetCamera);
        scene_type.set_function("get_entity", &Scene::GetEntity);

        camera_type.set_function("get_pos", &Camera::GetPos);
        camera_type.set_function("set_pos", &Camera::SetPos);
        camera_type.set_function("get_forward", &Camera::GetForward);
        camera_type.set_function("get_right", &Camera::GetRight);

        scene_type.set_function("add_terrain_physics", [](Scene* scene, const Entity ent) {
            if (!ent.HasComponent<Terrain>()) {
                VK_WARN("Entity does not have a terrain component. Can't add physics");
                return;
            }

            const auto& terrain = ent.GetComponent<Terrain>();

            System::BindScene(*scene);

            System::Physics_AddTerrain(terrain);
        });

        scene_type.set_function("get_physics",
                                [](const Scene* scene) -> ScenePhysics& { return *scene->scenePhysics; });

        camera_type.set_function("get_pitch", &Camera::GetPitch);
        camera_type.set_function("set_pitch", &Camera::SetPitch);

        camera_type.set_function("get_yaw", &Camera::GetYaw);
        camera_type.set_function("set_yaw", &Camera::SetYaw);
    }

    void RegisterGUIWindow(sol::state& lua, View::GUIWindow* gui) {
        auto gui_window_type =
            lua.new_usertype<View::GUIWindow>("gui");  // Creates a new usertype of the type 'View::GUIWindow'

        lua["GUI"] = gui;

        // REGISTERS C++ FUNCTIONS TO LUA
        gui_window_type.set_function("start_window", &View::GUIWindow::StartWindowCreation);

        gui_window_type.set_function("get_fps", &View::GUIWindow::GetFramesPerSecond);

        gui_window_type.set_function("add_text", &View::GUIWindow::AddText);
        gui_window_type.set_function("add_button", &View::GUIWindow::AddButton);
        gui_window_type.set_function("add_checkbox", &View::GUIWindow::AddCheckbox);

        gui_window_type.set_function("add_integer_slider", &View::GUIWindow::AddIntSlider);
        gui_window_type.set_function("add_float_slider", &View::GUIWindow::AddFloatSlider);

        gui_window_type.set_function("add_vector_integer_slider", &View::GUIWindow::AddVecIntSlider);
        gui_window_type.set_function("add_vector_float_slider", &View::GUIWindow::AddVecFloatSlider);

        gui_window_type.set_function("end_window", &View::GUIWindow::EndWindowCreation);
    }

    void RegisterRenderer([[maybe_unused]] sol::state& lua) {}

    void RegisterPhysics(sol::state& lua) {
        auto scenePhysicType = lua.new_usertype<ScenePhysics>("scenePhysics");

        auto rp3dVec3 = lua.new_usertype<rp3d::Vector3>("phyVec3");  // need for collider
        rp3dVec3["x"] = &rp3d::Vector3::x;
        rp3dVec3["y"] = &rp3d::Vector3::y;
        rp3dVec3["z"] = &rp3d::Vector3::z;

        auto rigidType = lua.new_usertype<RigidBody>("rigidBody");

        lua["BodyType"] =
            lua.create_table_with("Static", RigidBody::BODY_TYPE::STATIC, "Kinematic", RigidBody::BODY_TYPE::KINEMATIC,
                                  "Dynamic", RigidBody::BODY_TYPE::DYNAMIC);

        rigidType["BodyType"] = &RigidBody::Type;

        auto rigidDataType = lua.new_usertype<RigidBody::RigidData>("rigidData");

        rigidDataType["mass"] = &RigidBody::RigidData::mass;
        rigidDataType["gravity"] = &RigidBody::RigidData::grav;
        rigidDataType["linear_damp"] = &RigidBody::RigidData::LDamp;
        rigidDataType["angular_damp"] = &RigidBody::RigidData::ADamp;

        auto colliderType = lua.new_usertype<Collider>("collider");

        lua["Shape"] =
            lua.create_table_with("Box", Collider::ShapeName::BOX, "Sphere", Collider::ShapeName::SPHERE, "Capsule",
                                  Collider::ShapeName::CAPSULE, "TriangleMesh", Collider::ShapeName::TRIANGLE_MESH);

        colliderType["Shape"] = &Collider::ShapeName;

        auto ColliderBoundsType = lua.new_usertype<Collider::Bounds>("colliderBounds");

        ColliderBoundsType["min"] = &Collider::Bounds::min;
        ColliderBoundsType["max"] = &Collider::Bounds::max;
        ColliderBoundsType["center"] = &Collider::Bounds::center;
        ColliderBoundsType["extents"] = &Collider::Bounds::extents;
        ColliderBoundsType["radius"] = &Collider::Bounds::radius;

        rigidType.set_function("set_data",
                               [](RigidBody* rigid, const RigidBody::RigidData& data) {
                                   rigid->SetRigidData(data);
                               });

        rigidType.set_function("toggle_gravity", [](RigidBody* rigid) { rigid->ToggleGravity(); });

        rigidType.set_function("set_data",
                               [](RigidBody* rigid, const RigidBody::RigidData& data) { rigid->SetRigidData(data); });

        rigidType.set_function("toggle_gravity", [](RigidBody* rigid) { rigid->ToggleGravity(); });

        rigidType.set_function("set_body_type",
                               [](RigidBody* rigid, const RigidBody::BODY_TYPE type) { rigid->SetBodyType(type); });

        rigidType.set_function("set_velocity",
                               [](const RigidBody* rigid, const glm::vec3& vel) { rigid->SetVelocity(vel); });

        rigidType.set_function("set_angular_velocity",
                               [](const RigidBody* rigid, const glm::vec3& vel) { rigid->SetAngularVelocity(vel); });

        rigidType.set_function("set_linear_damp",
                               [](const RigidBody* rigid, const float damp) { rigid->SetLinearDamp(damp); });

        rigidType.set_function("set_angular_damp",
                               [](const RigidBody* rigid, const float damp) { rigid->SetAngularDamp(damp); });

        colliderType.set_function(
            "set_bounds", [](Collider* collider, const Collider::Bounds& bounds) { collider->SetBounds(bounds); });

        colliderType.set_function("set_bounds",
                                  [](Collider* collider, const Collider::Bounds& bounds) {
                                      collider->SetBounds(bounds);
                                  });

        scenePhysicType.set_function("enable_debug", &ScenePhysics::EnableDebug);
    }

    std::vector<glm::mat4> create_mat4_vector() { return {}; }

    void RegisterOther(sol::state& lua) { lua.set_function("vector_mat4", &create_mat4_vector); }
}  // namespace Vakol::Controller
