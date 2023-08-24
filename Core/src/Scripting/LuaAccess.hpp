#pragma once

#include <sol/sol.hpp>

namespace Vakol
{
    class ScriptEngine;
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

    void RegisterPrimitives(sol::state& lua); 

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
     * @brief Register the physics system in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterPhysics(sol::state& lua);

    /**
     * @brief Register the scene in Lua.
     *
     * @param lua The Lua state.
     * @param scriptEngine The script engine.
     */
    void RegisterScene(sol::state& lua, Vakol::ScriptEngine& scriptEngine);

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
} // namespace Vakol