#include "AssetLoader/AssetLoader.hpp"
#include "ECS/Components.hpp"
#include "ECS/Entity.hpp"
#include "LuaAccess.hpp"
#include "SceneManager/Scene.hpp"

namespace Vakol
{
    void RegisterEntity(sol::state& lua)
    {
        auto entity_type = lua.new_usertype<Entity>("Entity");

        entity_type.set_function("get_tag", [](Entity* ent) { return ent->GetComponent<Components::Tag>().tag; });
        entity_type.set_function("get_transform", &Entity::GetComponent<Components::Transform>);
        entity_type.set_function("get_fsm", &Entity::GetComponent<Components::FSM>);

        entity_type.set_function("create_height_map_terrain",
                                 [](Entity* ent, Scene& scene, std::string&& path, const float min, const float max) {
                                     if (!ent->HasComponent<Components::Drawable>())
                                         ent->AddComponent<Components::Drawable>();
                                     if (ent->HasComponent<Components::TerrainComp>())
                                         ent->RemoveComponent<Components::TerrainComp>();

                                     ent->AddComponent<Components::TerrainComp>();

                                     const auto& name = scene.getName();

                                     std::shared_ptr<Terrain> terrain = AssetLoader::GetTerrain(name);

                                     if (terrain == nullptr)
                                         terrain = AssetLoader::GetTerrain(name, path, min, max);

                                     auto& terrain_comp = ent->GetComponent<Components::TerrainComp>();

                                     terrain_comp.terrain_ptr = terrain;
                                     terrain_comp.min = min;
                                     terrain_comp.max = max;
                                     terrain_comp.path = std::move(path);
                                     terrain_comp.name = name;

                                     if (const auto& model = terrain->GetModel())
                                     {
                                         model->mesh().SetDrawMode(DRAW_MODE::STRIPS);
                                         model->mesh().SetDrawType(DRAW_TYPE::ELEMENTS);

                                         model->mesh().SetDrawModeInfo((terrain->GetSize() - 1) / 1); // num strips

                                         model->mesh().SetNumTrisPerStrip(terrain->GetSize() / 1 * 2 - 2);

                                         Components::Drawable& drawable = ent->GetComponent<Components::Drawable>();
                                         drawable.model_ptr = model;
                                     }

                                     return terrain;
                                 });

        entity_type.set_function("get_terrain", [](const Entity* ent) {
            if (ent->HasComponent<Components::TerrainComp>())
                return ent->GetComponent<Components::TerrainComp>().terrain_ptr;
        });

        entity_type.set_function("add_model", [](Entity* ent, const std::string& path, const float scale = 1.0f,
                                                 const bool animated = false, const bool backfaceCull = true) {
            if (!ent->HasComponent<Components::Drawable>())
                ent->AddComponent<Components::Drawable>();

            auto instance = false;

            auto [model, animator] = AssetLoader::GetModel(path, scale, animated, backfaceCull, instance);

            if (model)
            {
                auto& draw = ent->GetComponent<Components::Drawable>();

                draw.model_ptr = model;
                draw.name = path;
                draw.scale = scale;
                draw.animated = animated;
                draw.backfaceCull = backfaceCull;
                draw.instance = instance;

                if (animator && animated)
                {
                    if (!ent->HasComponent<Components::Animation>())
                        ent->AddComponent<Components::Animation>();

                    if (!instance)
                    {
                        if (!ent->HasComponent<Components::AnimatorComp>())
                            ent->AddComponent<Components::AnimatorComp>();

                        auto& _animator = ent->GetComponent<Components::AnimatorComp>();

                        _animator.attached_model = draw.name;
                        _animator.set(animator);
                    }

                    auto& animation = ent->GetComponent<Components::Animation>();
                    animation.attached_model = draw.name;
                }
            }

            return model;
        });

        entity_type.set_function("set_backface_culling", [](const Entity* ent, const bool cull) {
            if (!ent->HasComponent<Components::Drawable>())
                VK_ERROR("Cannot set backface culling without a drawable component!");

            ent->GetComponent<Components::Drawable>().backfaceCull = cull;
        });

        entity_type.set_function("get_model", [](const Entity* ent) {
            if (ent->HasComponent<Components::Drawable>())
                return ent->GetComponent<Components::Drawable>().model_ptr;
        });

        entity_type.set_function("active_model", [](const Entity* ent, bool active) {
            if (ent->HasComponent<Components::Drawable>())
                ent->GetComponent<Components::Drawable>().active = active;
        });

        entity_type.set_function("set_shader", [](const Entity* ent, const std::string& path) {
            if (!ent->HasComponent<Components::Drawable>())
            {
                VK_ERROR("Drawable Component is needed to set shader!");
                return;
            }

            const auto& model = ent->GetComponent<Components::Drawable>().model_ptr;
            const auto& shader = AssetLoader::GetShader(path);

            model->set_shader(shader);
        });

        entity_type.set_function("add_texture", [](const Entity* ent, const int mesh_index, const std::string& path,
                                                   const bool gamma, const bool flip) {
            if (!ent->HasComponent<Components::Drawable>())
            {
                VK_ERROR("Drawable component is needed to add texture to material!");
                return;
            }

            const auto& model = ent->GetComponent<Components::Drawable>().model_ptr;
            model->mesh(mesh_index).GetMaterial()->AddTexture(*AssetLoader::GetTexture(path, gamma, flip));
        });

        entity_type.set_function("add_raw_texture",
                                 [](const Entity* ent, const int mesh_index, const std::string& path) {
                                     if (!ent->HasComponent<Components::Drawable>())
                                     {
                                         VK_ERROR("Drawable component is needed to add texture to material!");
                                         return;
                                     }

                                     const auto& model = ent->GetComponent<Components::Drawable>().model_ptr;
                                     model->mesh(mesh_index).GetMaterial()->AddTexture(*AssetLoader::GetTexture(path));
                                 });

        entity_type.set_function("play_animation", [](const Entity* ent, const float animation_state) {
            if (!ent->HasComponent<Components::Animation>())
            {
                VK_ERROR("Animation component is needed to set it's animation state!");
                return;
            }

            auto& animation = ent->GetComponent<Components::Animation>();

            animation.state = static_cast<int>(animation_state);
        });

        entity_type.set_function("get_animation_duration", [](const Entity* ent, const int animation_state) {
            if (!ent->HasComponent<Components::Animation>())
            {
                VK_ERROR("Animation component is needed to get it's duration!");
                return -1.0;
            }

            const auto& animation = ent->GetComponent<Components::Animation>();

            return AssetLoader::GetAnimation(animation.attached_model, animation.state).duration_s();
        });

        entity_type.set_function("reset_animation", [](const Entity* ent, const int animation_state) {
            if (!ent->HasComponent<Components::Animation>())
            {
                VK_ERROR("Animation component is needed to reset it!");
                return;
            }

            const auto& animation = ent->GetComponent<Components::Animation>();

            return AssetLoader::GetAnimation(animation.attached_model, animation_state).reset_animation();
        });

        entity_type.set_function("physics_init", [](const Entity* ent, Scene& scene) {
            VK_CRITICAL("Physics init is deprecated!");
            // System::BindScene(scene);
            // System::Physics_InitEntity(*ent);
        });

        entity_type.set_function("add_rigid", [](Entity* ent) -> Components::RigidBody& {
            if (!ent->HasComponent<Components::RigidBody>())
                ent->AddComponent<Components::RigidBody>();

            return ent->GetComponent<Components::RigidBody>();
        });

        entity_type.set_function("get_rigid", [](const Entity* ent) -> Components::RigidBody& {
            if (ent->HasComponent<Components::RigidBody>())
                return ent->GetComponent<Components::RigidBody>();

            VK_CRITICAL("No rigid body component found on entity");
            assert(0);
        });

        entity_type.set_function("add_collider", [](Entity* ent) -> Components::Collider& {
            if (!ent->HasComponent<Components::Collider>())
                ent->AddComponent<Components::Collider>();
            return ent->GetComponent<Components::Collider>();
        });

        entity_type.set_function("get_collider", [](const Entity* ent) -> Components::Collider& {
            if (ent->HasComponent<Components::Collider>())
                return ent->GetComponent<Components::Collider>();

            VK_CRITICAL("No collider component found on entity");
            assert(0);
        });

        entity_type.set_function("get_bounds_from_model", [](const Entity* ent) -> void {
            if (ent->HasComponent<Components::Drawable, Components::Collider>())
            {
                const auto& model = ent->GetComponent<Components::Drawable>();

                auto& collider = ent->GetComponent<Components::Collider>();

                collider.bounds = GetBounds(model, ent->GetComponent<Components::Transform>());
            }
            else
            {
                VK_CRITICAL("drawable and collider must be present to get bounds from");
                assert(0);
            }
        });

        // TODO remove FSM component
        entity_type.set_function("add_fsm", [&](Entity* ent) -> Components::FSM& {
            if (!ent->HasComponent<Components::FSM>())
                ent->AddComponent<Components::FSM>(lua.create_table());
            return ent->GetComponent<Components::FSM>();
        });
    }
} // namespace Vakol