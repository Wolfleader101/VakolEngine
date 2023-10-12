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

    /**
     * @brief Registers the Time utility in the given Lua state, allowing scripts to access and manipulate time-related
     * functionalities.
     * @param lua Reference to the Lua state where the Time utility will be registered.
     */
    void RegisterTime(sol::state& lua);

    /**
     * @brief Registers the Input utility in the given Lua state, allowing scripts to access and manipulate
     * input-related functionalities.
     * @param lua Reference to the Lua state where the Input utility will be registered.
     */
    void RegisterInput(sol::state& lua);

    /**
     * @brief Registers the Model utility in the given Lua state, allowing scripts to access and manipulate
     * model-related functionalities.
     * @param lua Reference to the Lua state where the Model utility will be registered.
     */
    void RegisterModel(sol::state& lua);

    /**
     * @brief Registers the Mesh utility in the given Lua state, allowing scripts to access and manipulate mesh-related
     * functionalities.
     * @param lua Reference to the Lua state where the Mesh utility will be registered.
     */
    void RegisterMesh(sol::state& lua);

    /**
     * @brief Registers the Material utility in the given Lua state, allowing scripts to access and manipulate
     * material-related functionalities.
     * @param lua Reference to the Lua state where the Material utility will be registered.
     */
    void RegisterMaterial(sol::state& lua);

    /**
     * @brief Registers the Shader utility in the given Lua state, allowing scripts to access and manipulate
     * shader-related functionalities.
     * @param lua Reference to the Lua state where the Shader utility will be registered.
     */
    void RegisterShader(sol::state& lua);

    /**
     * @brief Registers the Transform utility in the given Lua state, allowing scripts to access and manipulate
     * transform-related functionalities.
     * @param lua Reference to the Lua state where the Transform utility will be registered.
     */
    void RegisterTransform(sol::state& lua);

    /**
     * @brief Registers the Terrain utility in the given Lua state, allowing scripts to access and manipulate
     * terrain-related functionalities.
     * @param lua Reference to the Lua state where the Terrain utility will be registered.
     */
    void RegisterTerrain(sol::state& lua);

    /**
     * @brief Registers the FSM (Finite State Machine) utility in the given Lua state, allowing scripts to access and
     * manipulate FSM-related functionalities.
     * @param lua Reference to the Lua state where the FSM utility will be registered.
     */
    void RegisterFSM(sol::state& lua);

    /**
     * @brief Registers the Camera utility in the given Lua state, allowing scripts to access and manipulate
     * camera-related functionalities.
     * @param lua Reference to the Lua state where the Camera utility will be registered.
     */
    void RegisterCamera(sol::state& lua);

    /**
     * @brief Registers the RigidBody utility in the given Lua state, allowing scripts to access and manipulate rigid
     * body-related functionalities.
     * @param lua Reference to the Lua state where the RigidBody utility will be registered.
     */
    void RegisterRigidBody(sol::state& lua);

    /**
     * @brief Registers the Collider utility in the given Lua state, allowing scripts to access and manipulate
     * collider-related functionalities.
     * @param lua Reference to the Lua state where the Collider utility will be registered.
     */
    void RegisterCollider(sol::state& lua);

    /**
     * @brief Register the entity in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterEntity(sol::state& lua, ScriptEngine& scriptEngine);

    /**
     * @brief Register the GUI window in Lua.
     *
     * @param lua The Lua state.
     */
    void RegisterGUIWindow(sol::state& lua);

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