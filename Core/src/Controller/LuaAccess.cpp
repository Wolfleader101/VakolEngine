#include "LuaAccess.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "AssetLoader/AssetLoader.hpp"
#include "Model/Assets/Material.hpp"
#include "Model/Components.hpp"
#include "Model/gl/GLInstance.hpp"

constexpr int DIRECTIONAL_LIGHT = 0;
constexpr int POINT_LIGHT = 1;
constexpr int SPOT_LIGHT = 2;

int OPTION = DIRECTIONAL_LIGHT;

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
        lua.set_function("load_texture", [](const std::string& path) {
            auto tex = AssetLoader::GetTexture(path);

            if (tex == nullptr) return false;

            return true;
        });

        lua.set_function("load_model", [](const std::string& path) {
            auto model = AssetLoader::GetModel(path);

            if (model == nullptr) return false;

            return true;
        });

        lua.set_function("load_shader", [](const std::string& path) {
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
        auto modelType = lua.new_usertype<Assets::Model>("model");
        auto meshType = lua.new_usertype<Assets::Mesh>("mesh");
        auto materialType = lua.new_usertype<Assets::Material>("material");
        auto textureType = lua.new_usertype<Assets::Texture>("texture");
        auto shaderType = lua.new_usertype<Shader>("shader");

        lua.set_function("raw_texture", [](const std::string& path) { return Texture(path); });

        lua.set_function("texture", [](const std::string& path, const bool gamma, const bool flip) {
            return Texture(path, gamma, flip);
        });

        entityType.set_function("get_transform", &Entity::GetComponent<Model::Components::Transform>);

        entityType.set_function("add_heightmap_terrain", [](Entity* ent, const std::string& path) {
            if (!ent->HasComponent<Model::Components::Drawable>()) ent->AddComponent<Model::Components::Drawable>();

            if (ent->HasComponent<Terrain>()) ent->RemoveComponent<Terrain>();

            ent->AddComponent<Terrain>(path);

            auto terrain = ent->GetComponent<Terrain>();

            auto model = terrain.GetModel();  // doesn't that look nice?

            const auto size = terrain.GetSize();

            if (model) {
                model->GetMesh().GetVertexArray()->SetStrips((size - 1) / 1, (size / 1) * 2 - 2);

                ent->GetComponent<Model::Components::Drawable>().model_ptr = model;
            }

            return terrain;
        });

        entityType.set_function(
            "add_fault_formation_terrain", [](Entity* ent, const int size, const int iterations, const float filter,
                                              const bool random, const int minHeight, const int maxHeight) {
                if (!ent->HasComponent<Model::Components::Drawable>()) ent->AddComponent<Model::Components::Drawable>();
                if (ent->HasComponent<Terrain>()) ent->RemoveComponent<Terrain>();

                ent->AddComponent<Terrain>(size, iterations, filter, random, minHeight, maxHeight);

                auto terrain = ent->GetComponent<Terrain>();

                auto model = terrain.GetModel();  // doesn't that look nice?

                if (model) {
                    model->GetMesh().GetVertexArray()->SetStrips((size - 1) / 1, (size / 1) * 2 - 2);

                    ent->GetComponent<Model::Components::Drawable>().model_ptr = model;
                }

                return terrain;
            });

        entityType.set_function("add_clod_terrain", [](Entity* ent, const int size) {
            if (!ent->HasComponent<Model::Components::Drawable>()) ent->AddComponent<Model::Components::Drawable>();
            if (ent->HasComponent<Terrain>()) ent->RemoveComponent<Terrain>();

            ent->AddComponent<Terrain>(size);

            auto terrain = ent->GetComponent<Terrain>();

            auto model = terrain.GetModel();  // doesn't that look nice?

            if (model) {
                model->GetMesh().GetVertexArray()->SetPatches(400, 4);

                ent->GetComponent<Model::Components::Drawable>().model_ptr = model;
            }

            return terrain;
        });

        entityType.set_function("add_model", [](Entity* ent, const std::string& path) {
            if (!ent->HasComponent<Model::Components::Drawable>()) ent->AddComponent<Model::Components::Drawable>();

            auto model = AssetLoader::GetModel(path);

            if (model) {
                ent->GetComponent<Model::Components::Drawable>().model_ptr = model;
                return model;
            }
        });

        entityType.set_function("get_model", [](Entity* ent) {
            if (ent->HasComponent<Model::Components::Drawable>()) {
                return ent->GetComponent<Model::Components::Drawable>().model_ptr;
            }
        });

        modelType.set_function("get_mesh_count", &Assets::Model::GetMeshCount);
        modelType.set_function("get_mesh", &Assets::Model::GetMesh);

        modelType.set_function("set_shader", &Assets::Model::SetShader);
        modelType.set_function("get_shader", &Assets::Model::GetShader);

        meshType.set_function("get_material", &Assets::Mesh::GetMaterial);

        materialType.set_function("add_texture", &Assets::Material::AddTexture);
        materialType.set_function("get_texture", &Assets::Material::GetTexture);

        // materialType.set_function("get_ambient", &Assets::Material::GetAmbientColor);
        // materialType.set_function("get_diffuse", &Assets::Material::GetDiffuseColor);
        // materialType.set_function("get_specular", &Assets::Material::GetSpecularColor);
        // materialType.set_function("get_shininess", &Assets::Material::GetShininess);

        // materialType.set_function("set_ambient", &Assets::Material::SetAmbientColor);
        // materialType.set_function("set_diffuse", &Assets::Material::SetDiffuseColor);
        // materialType.set_function("set_specular", &Assets::Material::SetSpecularColor);
        // materialType.set_function("set_shininess", &Assets::Material::SetShininess);

        textureType.set_function("bind_texture", &Assets::Texture::Bind);
        textureType.set_function("unbind_texture", &Assets::Texture::Unbind);

        shaderType.set_function("set_int", &Assets::Shader::SetInt);
        shaderType.set_function("set_bool", &Assets::Shader::SetBool);
        shaderType.set_function("set_float", &Assets::Shader::SetFloat);
        shaderType.set_function("set_vec2", &Assets::Shader::SetVec2);
        shaderType.set_function("set_vec3", &Assets::Shader::SetVec3);
        shaderType.set_function("set_vec4", &Assets::Shader::SetVec4);
    }

    void RegisterECS(sol::state& lua) {
        auto TransformType = lua.new_usertype<Model::Components::Transform>("transform");
        auto terrainType = lua.new_usertype<Terrain>("terrain");

        TransformType["pos"] = &Model::Components::Transform::pos;
        TransformType["rot"] = &Model::Components::Transform::rot;
        TransformType["scale"] = &Model::Components::Transform::scale;

        // terrainType.set_function("load_heightmap", &Terrain::LoadHeightMap);
        // terrainType.set_function("load_texture", &Terrain::LoadTexture);
        // terrainType.set_function("generate", &Terrain::Generate);

        terrainType.set_function("get_height", &Terrain::GetHeight);
        terrainType.set_function("get_size", &Terrain::GetSize);
        terrainType.set_function("get_model", &Terrain::GetModel);
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

    void RegisterWindow(sol::state& lua) {}
    void RegisterRenderer(sol::state& lua) {}
    void RegisterPhysics(sol::state& lua) {}
}  // namespace Vakol::Controller
