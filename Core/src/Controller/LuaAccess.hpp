#pragma once

#include <sol/sol.hpp>

#include "Application.hpp"
#include "Scene.hpp"

namespace Vakol::Controller {
    /**
     * @brief Register the asset loader in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterAssetLoader(sol::state& lua);

    /**
     * @brief Register the logger in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterLogger(sol::state& lua);

    void RegisterTime(sol::state& lua);

    void RegisterInput(sol::state& lua);

    void RegisterModel(sol::state& lua);

    void RegisterMesh(sol::state& lua);

    void RegisterMaterial(sol::state& lua);

    void RegisterShader(sol::state& lua);

    void RegisterTransform(sol::state& lua);

    void RegisterTerrain(sol::state& lua);

    void RegisterFSM(sol::state& lua);

    void RegisterCamera(sol::state& lua);

    void RegisterRigidBody(sol::state& lua);

    void RegisterCollider(sol::state& lua);

    /**
     * @brief Register the application in Lua.
     *
     * @param lua The Lua state.
     * @param app The pointer to the application.
     */
    void RegisterApplication(sol::state& lua, Application* app);

    /**
     * @brief Register the entity in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterEntity(sol::state& lua);

    /**
     * @brief Register the GUI window in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterGUIWindow(sol::state& lua);

    /**
     * @brief Register the renderer in Lua.
     *
     * @param lua The Lua state.
     * @param renderer The shared pointer to the renderer.
     */
    void RegisterRenderer(sol::state& lua, const std::shared_ptr<View::Renderer>& renderer);

    /**
     * @brief Register the physics system in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterPhysics(sol::state& lua);

    /**
     * @brief Register the scene in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterScene(sol::state& lua);

    /**
     * @brief Register the math library in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterMath(sol::state& lua);

    /**
     * @brief Register other miscellaneous functions in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterOther(sol::state& lua);
}  // namespace Vakol::Controller