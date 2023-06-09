#pragma once

#include <sol/sol.hpp>

#include "Application.hpp"
#include "Scene.hpp"


namespace Vakol::Controller
{
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
     * @param state The shared pointer to the Lua state.
     * @param lua The Lua state.
     */
    void RegisterEntity(std::shared_ptr<LuaState>& state, sol::state& lua);

    /**
     * @brief Register the ECS in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterECS(sol::state& lua);

    /**
     * @brief Register the GUI window in Lua.
     *
     * @param lua The Lua state.
     * @param gui The pointer to the GUI window.
     */
    void RegisterGUIWindow(sol::state& lua, View::GUIWindow* gui);

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