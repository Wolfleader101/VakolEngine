#pragma once

#include <Controller/EntityList.hpp>
#include <Controller/Physics/ScenePhysics.hpp>
#include <Model/Components.hpp>

#include "Controller/EntityList.hpp"
#include "Controller/Terrain.hpp"
#include "LuaState.hpp"
#include "Scene.hpp"
#include "View/Renderer/Renderer.hpp"

namespace Vakol::Controller {

    class Scene;
    using namespace Vakol::Controller::Physics;
    using namespace Vakol::Model::Components;

    class System {
       public:
        System() = delete;

        static void BindScene(Scene& scene);

        // -- READ --
        // if you add a function use the convention Type_Action()
        // will make it more intuitive to find names of funcs
        static void Drawable_Init();

        static void Drawable_Update(const Time& time, const Controller::Camera& camera,
                                    const std::shared_ptr<View::Renderer> renderer);

        static void Script_Update(LuaState& lua, EntityList& list, Scene* scene);

        static void Physics_InitObject(
            RigidBody& rigid, std::optional<std::reference_wrapper<Collider>> collider,
            std::optional<std::reference_wrapper<Drawable>> model,  // only used if you want to put triangle mesh
            const Transform& trans);

        static void Physics_Init();
        static void Physics_UpdateTransforms(float factor);
        static void Physics_SerializationPrep();

       private:
        static std::shared_ptr<ScenePhysics> m_SP;
        static entt::registry* m_registry;
    };
}  // namespace Vakol::Controller