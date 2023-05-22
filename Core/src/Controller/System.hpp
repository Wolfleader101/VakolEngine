#pragma once

#include <Controller/EntityList.hpp>
#include <Controller/Physics/ScenePhysics.hpp>
#include <Model/Components.hpp>

#include "Controller/Terrain.hpp"
#include "LuaState.hpp"
#include "Scene.hpp"
#include "View/Renderer/Renderer.hpp"

namespace Vakol::Controller
{

    class Scene;
    using namespace Physics;
    using namespace Components;

    class System
	{
    public:
        System() = delete;

        static void BindScene(Scene& scene);

        // -- READ --
        // if you add a function use the convention Type_Action()
        // will make it more intuitive to find names of funcs

        static void Drawable_Update(const Time& time, const Camera& camera, const std::shared_ptr<View::Renderer>& renderer);

        static void Script_Update(LuaState& lua, EntityList& list, Scene* scene);

        static void Physics_InitEntity(Entity& ent);

        static void Physics_Init();
        static void Physics_UpdateTransforms(float factor);
        static void Physics_SerializationPrep();
        static void Physics_AddTerrain(const Terrain& ter); 

    private:
        static std::shared_ptr<ScenePhysics> m_SP;
        static entt::registry* m_registry;
        static EntityList* Entlist;
    };
}  // namespace Vakol::Controller