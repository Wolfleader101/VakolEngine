#include "LuaAccess.hpp"

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

#include "AssetLoader/AssetLoader.hpp"
#include "AssetLoader/TextureLoader.hpp"
#include "Model/Assets/Material.hpp"
#include "Model/Components.hpp"
#include "Model/Instance.hpp"
#include "System.hpp"
#include "View/GUI/GUIWindow.hpp"


namespace Vakol::Controller 
{
    std::unordered_map<std::string, Components::Animator> s_animator_map;

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
            vec3.set_function("normalize", [](const glm::vec3& v) -> glm::vec3 { return glm::normalize(v); });
            vec3.set_function("dot", [](const glm::vec3& u, const glm::vec3& v) -> float { return glm::dot(u, v); });
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

        {
            sol::constructors<glm::quat(), glm::quat(glm::vec3)> ctor;

            auto quat = lua.new_usertype<glm::quat>("Quaternion", ctor);

            quat.set_function("Euler", [](const glm::quat& rot) { return eulerAngles(rot); });
        }
    }

    void RegisterLogger(sol::state& lua) {
        lua.set_function("print", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintTrace(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintTrace(std::to_string(va[0].get<float>()));
            else if (arg.get_type() == sol::type::boolean)
                Logger::ScriptPrintTrace(std::to_string(va[0].get<bool>()));
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
        lua.set_function("load_texture", [](const std::string& path, const bool gamma, const bool flip) {
            if (const auto texture = AssetLoader::GetTexture(path, gamma, flip); texture == nullptr) return false;

            return true;
        });

        lua.set_function("load_model", [](const std::string& path, const float scale = 1.0f,
                                          const bool animated = false, const bool backfaceCull = true) {
            if (const auto& [model, animator] = AssetLoader::GetModel(path, scale, animated, backfaceCull);
                model == nullptr)
                return false;

            return true;
        });

        lua.set_function("load_shader", [](const std::string& path) {
            if (const auto& shader = AssetLoader::GetShader(path); shader == nullptr) return false;

            return true;
        });
    }

    void RegisterApplication(sol::state& lua, Application* app) {
        lua.set_function("app_run", &Application::SetRunning, app);
        lua.set_function("add_scene", &Application::AddScene, app);
        lua.set_function("get_scene", &Application::GetScene, app);

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
        input_type.set_function("get_mouse", &Input::GetMouseButton);
        input_type.set_function("get_mouse_down", &Input::GetMouseButtonDown);
        input_type.set_function("get_mouse_up", &Input::GetMouseButtonUp);
        input_type.set_function("get_mouse_pos", &Input::GetMousePos);
        input_type.set_function("get_delta_mouse_pos", &Input::GetDeltaMousePos);

        lua["Input"] = &app->GetInput();

        lua["KEYS"] = lua.create_table_with(
            "MOUSE_0", Input::KEY::KEY_MOUSE_0, "MOUSE_1", Input::KEY::KEY_MOUSE_1, 
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
            "KEY_Z", Input::KEY::KEY_Z, "KEY_LEFT_SHIFT", Input::KEY::KEY_LEFT_SHIFT, "KEY_ESC",
            Input::KEY::KEY_ESCAPE);
    }

    void RegisterEntity(std::shared_ptr<LuaState>& state, sol::state& lua)
    {
        auto entity_type = lua.new_usertype<Entity>("entity");
        auto model_type = lua.new_usertype<Assets::Model>("model");
        auto mesh_type = lua.new_usertype<Mesh>("mesh");
        auto material_type = lua.new_usertype<Assets::Material>("material");
        auto shader_type = lua.new_usertype<Shader>("shader");

        lua.set_function("instantiate_model",
                         [](const std::shared_ptr<Assets::Model>& model, const std::vector<glm::mat4>& matrices,
                            const int amount) { CreateInstances(model->meshes(), matrices, amount, 3); });

        entity_type.set_function("get_tag", [](Entity* ent) { return ent->GetComponent<Tag>().tag; });
        entity_type.set_function("get_transform", &Entity::GetComponent<Transform>);
        entity_type.set_function("get_fsm", &Entity::GetComponent<FSM>); 

        entity_type.set_function("create_height_map_terrain",
                                 [](Entity* ent, Scene& scene, std::string&& path, const float min, const float max) {
                                     if (!ent->HasComponent<Drawable>()) ent->AddComponent<Drawable>();
                                     if (ent->HasComponent<Components::Terrain>()) ent->RemoveComponent<Components::Terrain>();

                                     ent->AddComponent<Components::Terrain>();
                                     
                                     const auto& name = scene.getName();

                                     std::shared_ptr<Terrain> terrain = AssetLoader::GetTerrain(name);

                                     if(terrain == nullptr) terrain = AssetLoader::GetTerrain(name, path, min, max);

                                     auto& terrain_comp = ent->GetComponent<Components::Terrain>();

                                     terrain_comp.terrain_ptr = terrain;
                                     terrain_comp.min = min;
                                     terrain_comp.max = max;
                                     terrain_comp.path = std::move(path);
                                     terrain_comp.name = name;


                                     if (const auto& model = terrain->GetModel()) {
                                         model->mesh().SetDrawMode(DRAW_MODE::STRIPS);
                                         model->mesh().SetDrawType(DRAW_TYPE::ELEMENTS);

                                         model->mesh().SetDrawModeInfo((terrain->GetSize() - 1) / 1);  // num strips

                                         model->mesh().SetNumTrisPerStrip(terrain->GetSize() / 1 * 2 - 2);


                                        Drawable& drawable = ent->GetComponent<Drawable>();
                                        drawable.model_ptr = model;
                                    }

                                     return terrain;
                                 });

        // entity_type.set_function("create_clod_terrain",
        //                          [](Entity* ent, std::string&& path, const float min, const float max) {
        //                              if (!ent->HasComponent<Drawable>()) ent->AddComponent<Drawable>();

        //                              if (ent->HasComponent<Terrain>()) ent->RemoveComponent<Terrain>();

        //                              ent->AddComponent<Terrain>(LoadCLODTerrain(std::move(path), min, max));

        //                              auto& terrain = ent->GetComponent<Terrain>();

        //                              if (const auto& model = terrain.GetModel()) {
        //                                  model->mesh().SetDrawMode(DRAW_MODE::PATCHES);
        //                                  model->mesh().SetDrawType(DRAW_TYPE::ARRAYS);

        //                                  model->mesh().SetDrawModeInfo(400);  // num patches

        //                                  ent->GetComponent<Drawable>().model_ptr = model;
        //                              }

        //                              return terrain;
        //                          });

        entity_type.set_function("get_terrain", [](const Entity* ent) {
            if (ent->HasComponent<Components::Terrain>()) return ent->GetComponent<Components::Terrain>().terrain_ptr;
        });

        entity_type.set_function("add_model", [](Entity* ent, const std::string& path, const float scale = 1.0f,
                                                 const bool animated = false, const bool backfaceCull = true) {
            if (!ent->HasComponent<Drawable>()) ent->AddComponent<Drawable>();

            auto instance = false;

            auto [model, animator] = AssetLoader::GetModel(path, scale, animated, backfaceCull, instance);

            if (model) 
            {
                auto& draw = ent->GetComponent<Drawable>();

                draw.model_ptr = model;
                draw.name = path;
                draw.scale = scale;
                draw.animated = animated;
                draw.backfaceCull = backfaceCull;
                draw.instance = instance;

                if (animator && animated) 
                {
                    if (!ent->HasComponent<Components::Animation>()) ent->AddComponent<Components::Animation>();
                    
                    if (!instance)
                    {   
                        if (!ent->HasComponent<Components::Animator>()) ent->AddComponent<Components::Animator>();

                        auto& _animator = ent->GetComponent<Components::Animator>();

                        _animator.attached_model = draw.name;
                        _animator.set(animator);

                        s_animator_map[_animator.attached_model] = _animator;
                    }

                    auto& animation = ent->GetComponent<Components::Animation>();
                    animation.attached_model = draw.name;
                }
            }

            return model;
        });

        entity_type.set_function("get_model", [](const Entity* ent) {
            if (ent->HasComponent<Drawable>()) return ent->GetComponent<Drawable>().model_ptr;
        });

        entity_type.set_function("set_shader", [](const Entity* ent, const std::string& path) {
            if (!ent->HasComponent<Drawable>()) {
                VK_ERROR("Drawable Component is needed to set shader!");
                return;
            }

            const auto& model = ent->GetComponent<Drawable>().model_ptr;
            const auto& shader = AssetLoader::GetShader(path);

            model->set_shader(shader);
        });

        entity_type.set_function("add_texture", [](const Entity* ent, const int mesh_index, const std::string& path,
                                                   const bool gamma, const bool flip) {
            if (!ent->HasComponent<Drawable>()) {
                VK_ERROR("Drawable component is needed to add texture to material!");
                return;
            }

            const auto& model = ent->GetComponent<Drawable>().model_ptr;
            model->mesh(mesh_index).GetMaterial()->AddTexture(*AssetLoader::GetTexture(path, gamma, flip));
        });

        entity_type.set_function("add_raw_texture",
                                 [](const Entity* ent, const int mesh_index, const std::string& path) {
                                     if (!ent->HasComponent<Drawable>()) {
                                         VK_ERROR("Drawable component is needed to add texture to material!");
                                         return;
                                     }

                                     const auto& model = ent->GetComponent<Drawable>().model_ptr;
                                     model->mesh(mesh_index).GetMaterial()->AddTexture(*AssetLoader::GetTexture(path));
                                 });

        entity_type.set_function("play_animation", [](const Entity* ent, const int animation_state) 
        {
            if (!ent->HasComponent<Components::Animation>()) 
            {
                VK_ERROR("Animation component is needed to set it's animation state!");
                return;
            }

            auto& animation = ent->GetComponent<Components::Animation>();

            animation.state = animation_state;
        });

        entity_type.set_function("get_animation_duration", [](const Entity* ent, const int animation_state) 
        {
            if (!ent->HasComponent<Components::Animation>()) 
            {
                VK_ERROR("Animation component is needed to get it's duration!");
                return -1.0f;
            }

            const auto& animation = ent->GetComponent<Components::Animation>();

            return s_animator_map.at(animation.attached_model).c_animation(animation_state).duration_s();
        });

        entity_type.set_function("reset_animation", [](const Entity* ent, const int animation_state) 
        {
            if (!ent->HasComponent<Components::Animation>()) 
            {
                VK_ERROR("Animation component is needed to reset it!");
                return;
            }

            const auto& animation = ent->GetComponent<Components::Animation>();

            s_animator_map.at(animation.attached_model).animation(animation_state).reset_animation();
        });

        model_type.set_function("get_mesh_count", &Assets::Model::nMeshes);
        model_type.set_function("get_mesh", &Assets::Model::mesh);

        model_type.set_function("get_shader", &Assets::Model::shader);

        mesh_type.set_function("get_material", &Mesh::GetMaterial);

        material_type.set_function("get_texture", &Assets::Material::GetTexture);

        material_type.set_function("get_ambient_color", &Assets::Material::GetAmbientColor);
        material_type.set_function("get_diffuse_color", &Assets::Material::GetDiffuseColor);

        shader_type.set_function("set_bool", [](const Shader* shader, const std::string& name, const bool value) {
            shader->Bind();
            shader->SetBool(name.c_str(), value);
            shader->Unbind();
        });

        shader_type.set_function("set_int", [](const Shader* shader, const std::string& name, const int value) {
            shader->Bind();
            shader->SetInt(name.c_str(), value);
            shader->Unbind();
        });

        shader_type.set_function("set_float", [](const Shader* shader, const std::string& name, const float value) {
            shader->Bind();
            shader->SetFloat(name.c_str(), value);
            shader->Unbind();
        });

        shader_type.set_function("set_vec2v",
                                 sol::resolve<void(const char*, const glm::vec2&) const>(&Shader::SetVec2));
        shader_type.set_function("set_vec3v",
                                 sol::resolve<void(const char*, const glm::vec3&) const>(&Shader::SetVec3));
        shader_type.set_function("set_vec4v",
                                 sol::resolve<void(const char*, const glm::vec4&) const>(&Shader::SetVec4));

        shader_type.set_function("set_vec2", sol::resolve<void(const char*, float, float) const>(&Shader::SetVec2));
        shader_type.set_function("set_vec3",
                                 sol::resolve<void(const char*, float, float, float) const>(&Shader::SetVec3));
        shader_type.set_function("set_vec4",
                                 sol::resolve<void(const char*, float, float, float, float) const>(&Shader::SetVec4));

        entity_type.set_function("physics_init", [](const Entity* ent, Scene& scene) {
            System::BindScene(scene);
            System::Physics_InitEntity(*ent);
        });

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

        entity_type.set_function("get_bounds_from_model", [](const Entity* ent) -> void {
            if (ent->HasComponent<Drawable, Collider>()) {
                const auto& model = ent->GetComponent<Drawable>();

                auto& collider = ent->GetComponent<Collider>();

                collider.bounds = GetBounds(model, ent->GetComponent<Transform>());
            } else {
                VK_CRITICAL("drawable and collider must be present to get bounds from");
                assert(0);
            }
        });

        entity_type.set_function("add_fsm", [&state](Entity* ent) -> FSM& {
            
            if (!ent->HasComponent<FSM>()) ent->AddComponent<FSM>(state);
            return ent->GetComponent<FSM>();
        });
    }

    void RegisterECS(sol::state& lua) {
        auto transform_type = lua.new_usertype<Transform>("transform");

        transform_type["pos"] = &Transform::pos;
        transform_type["rot"] = &Transform::eulerAngles;
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

        scene_type.set_function("set_active", [](Scene* scene, const bool active) { scene->active = active; });

        scene_type.set_function("get_camera", &Scene::GetCamera);
        scene_type.set_function("get_entity", &Scene::GetEntity);

        camera_type.set_function("get_pos", &Camera::GetPos);
        camera_type.set_function("set_pos", &Camera::SetPos);
        camera_type.set_function("get_forward", &Camera::GetForward);
        camera_type.set_function("get_right", &Camera::GetRight);

        scene_type.set_function("add_terrain_physics", [](Scene* scene, const Entity ent) {
            if (!ent.HasComponent<Components::Terrain>()) {
                VK_WARN("Entity does not have a terrain component. Can't add physics");
                return;
            }

            const auto& terrain = ent.GetComponent<Components::Terrain>();

            System::BindScene(*scene);

            System::Physics_AddTerrain(*terrain.terrain_ptr);
        });

        scene_type.set_function("enable_debug",
                                [](Scene* scene, const bool enable) { scene->scenePhysics->EnableDebug(enable); });

        scene_type.set_function("serialize",
                                &Scene::Serialize);  // Give it folder assets/scenes. will create subfolder for scene
        scene_type.set_function(
            "deserialize",
            &Scene::Deserialize);  // needs to be given folder assets/scenes/scene_name .ie assets/scenes/Test Scene
        scene_type.set_function("get_name", &Scene::getName);

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
        gui_window_type.set_function("get_display_window_width", &View::GUIWindow::DisplayWindowWidth);
        gui_window_type.set_function("get_display_window_height", &View::GUIWindow::DisplayWindowHeight);

        gui_window_type.set_function("start_window", &View::GUIWindow::StartWindowCreation);

        gui_window_type.set_function("update", &View::GUIWindow::Update);

        gui_window_type.set_function("start_frame", &View::GUIWindow::CreateNewFrame);
        gui_window_type.set_function("end_frame", &View::GUIWindow::EndFrame);

        gui_window_type.set_function("get_fps", &View::GUIWindow::GetFramesPerSecond);

        gui_window_type.set_function("add_text", &View::GUIWindow::AddText);
        gui_window_type.set_function("add_button", &View::GUIWindow::AddButton);
        gui_window_type.set_function("add_checkbox", &View::GUIWindow::AddCheckbox);

        gui_window_type.set_function(
            "add_image", [](const View::GUIWindow* GUI, const std::string& path, const float width, const float height,
                            const bool centerX, const bool centerY) {
                const auto& tex = AssetLoader::GetTexture(path, false, false);
                const unsigned int texID = tex->GetID();

                GUI->AddImage(texID, {width, height}, centerX, centerY);
            });

        gui_window_type.set_function("add_integer_slider", &View::GUIWindow::AddIntSlider);
        gui_window_type.set_function("add_float_slider", &View::GUIWindow::AddFloatSlider);

        gui_window_type.set_function("add_vector_integer_slider", &View::GUIWindow::AddVecIntSlider);
        gui_window_type.set_function("add_vector_float_slider", &View::GUIWindow::AddVecFloatSlider);

        gui_window_type.set_function("same_line", &View::GUIWindow::SameLine);

        gui_window_type.set_function("change_background_colour", &View::GUIWindow::WindowBackgroundStyle);
        gui_window_type.set_function("change_background_rounding", &View::GUIWindow::WindowRoundingStyle);

        gui_window_type.set_function("end_window", &View::GUIWindow::EndWindowCreation);
    }

    void RegisterRenderer(sol::state& lua, const std::shared_ptr<View::Renderer>& renderer) {
        lua.set_function("toggle_wireframe", [&] { renderer->ToggleWireframe(); });

        lua.set_function("toggle_skybox", [&] { renderer->ToggleSkybox(); });

        lua.set_function("clear_color_v", [&](const glm::vec4& color) { renderer->ClearColor(color); });

        lua.set_function("clear_color", [&](const float r, const float g, const float b, const float a) {
            renderer->ClearColor(r, g, b, a);
            renderer->ClearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        });
    }

    void RegisterPhysics(sol::state& lua) {
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
                               [](RigidBody* rigid, const RigidBody::RigidData& data) { rigid->SetRigidData(data); });

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

        colliderType.set_function(
            "set_bounds", [](Collider* collider, const Collider::Bounds& bounds) { collider->SetBounds(bounds); });
    }

    std::vector<glm::mat4> create_mat4_vector(const int reserve) {
        std::vector<glm::mat4> vector;

        vector.reserve(reserve);

        return vector;
    }

    void RegisterOther(sol::state& lua) { lua.set_function("vector_mat4", &create_mat4_vector); }
}  // namespace Vakol::Controller
