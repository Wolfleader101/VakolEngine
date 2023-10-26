#pragma once

#include <string>
#include <vector>

#include "ScriptTypes.hpp"

namespace Vakol
{

    // class EngineSystem {  // interface for engine system, something to think about
    //    public:
    //     virtual void Enable() = 0;
    //     virtual void Disable() = 0;
    //     virtual void Init() = 0;  //? could take in registry shared ptr and time?
    //     virtual void Update() = 0;
    //     virtual void Tick() = 0;
    // };

    class Entity;

    /**
     * @brief ScriptEngine class to manage and manipulate Lua scripts, variables, tables, and functions.
     */
    class ScriptEngine
    {
      public:
        /**
         * @brief Default constructor initializing the script engine.
         */
        ScriptEngine();

        /**
         * @brief Creates a Lua script from a file path. (Deprecated)
         * @warning Global access to the Lua state through this method is deprecated.
         * @param name The name of the Lua script.
         * @param scriptPath Path to the Lua script file.
         * @return LuaScript object representing the Lua script.
         */
        LuaScript CreateScript(const std::string& name, const std::string& scriptPath);

        /**
         * @brief Creates a new Lua table.
         * @return LuaTable object representing the newly created Lua table.
         */
        LuaTable CreateTable();

        /**
         * @brief Retrieves a global variable from the Lua state.
         * @param name The name of the global variable.
         * @return LuaType object holding the global variable.
         */
        LuaType GetGlobalVariable(const std::string& name);

        /**
         * @brief Template function to set a global variable in the Lua state.
         * @tparam T Type of the value to set.
         * @param name The name of the global variable.
         * @param value The value to set the global variable to.
         */
        template <typename T>
        void SetGlobalVariable(const std::string& name, T&& value)
        {
            m_state[name] = std::forward<T>(value);
        }

        /**
         * @brief Retrieves a variable from a specific Lua script.
         * @param script The Lua script to retrieve the variable from.
         * @param varName The name of the variable to retrieve.
         * @return LuaType object holding the variable.
         */
        LuaType GetScriptVariable(const LuaScript& script, const std::string& varName);

        /**
         * @brief Template function to set a variable in a specific Lua script.
         * @tparam T Type of the value to set.
         * @param script The Lua script to set the variable in.
         * @param varName The name of the variable to set.
         * @param value The value to set the variable to.
         */
        template <typename T>
        void SetScriptVariable(LuaScript& script, const std::string& varName, T&& value)
        {
            script.env[varName] = std::forward<T>(value);
        }

        /**
         * @brief Template function to set a global function in the Lua state.
         * @tparam Args Types of the arguments the function takes.
         * @tparam Key Type of the function name parameter.
         * @param key The name of the global function to set.
         * @param args The arguments of the global function to set.
         */
        template <typename... Args, typename Key>
        void SetGlobalFunction(Key&& key, Args&&... args)
        {
            m_state.set_function(std::forward<Key>(key), std::forward<Args>(args)...);
        }

        /**
         * @brief Template function to set a function in a specific Lua script.
         * @tparam Args Types of the arguments the function takes.
         * @tparam Key Type of the function name parameter.
         * @param script The Lua script to set the function in.
         * @param key The name of the function to set.
         * @param args The arguments of the function to set.
         */
        template <typename... Args, typename Key>
        void SetScriptFunction(LuaScript& script, Key&& key, Args&&... args)
        {
            script.env.set_function(std::forward<Key>(key), std::forward<Args>(args)...);
        }

        /**
         * @brief Initializes a Lua script, preparing it for execution.
         * @param script The Lua script to initialize.
         */
        void InitScript(LuaScript& script);

        /**
         * @brief Updates a Lua script, potentially modifying its state or triggering behaviors based on the update.
         * @param script The Lua script to update.
         */
        void UpdateScript(LuaScript& script);

        /**
         * @brief Ticks a Lua script, advancing its state or triggering behaviors based on the tick.
         * @param script The Lua script to tick.
         */
        void TickScript(LuaScript& script);

        /**
         * @brief Updates a Lua script in relation to physics, potentially modifying its state or triggering
         * physics-based behaviors.
         * @param script The Lua script to update.
         */
        void PhysUpdateScript(LuaScript& script);

        void PhysContactCallback(LuaScript& script, std::shared_ptr<Entity> ent);

      private:
        /**
         * @brief The global Lua state managed by the script engine.
         */
        LuaState m_state;

        /**
         * @brief Retrieves a variable from a specific Lua environment.
         * @param env The Lua environment to retrieve the variable from.
         * @param varName The name of the variable to retrieve.
         * @return LuaType object holding the variable.
         */
        LuaType GetVariable(sol::environment env, const std::string& varName);

        /**
         * @brief Runs a Lua file in a specific Lua environment.
         * @param env The Lua environment to run the file in.
         * @param file The Lua file to run.
         */
        void RunFile(sol::environment env, const std::string& file);

        /**
         * @brief Runs a Lua function in a specific Lua environment.
         * @param env The Lua environment to run the function in.
         * @param funcName The name of the function to run.
         * @param ignoreMissing Flag to ignore if the function is missing in the environment.
         * @param args The arguments to pass to the function.
         */
        void RunFunction(sol::environment env, const std::string& funcName, bool ignoreMissing = true,
                         const std::vector<LuaType>& args = {});

        /**
         * @brief Registers native functions to be available in the Lua environment.
         */
        void RegisterFunctions();

        /**
         * @brief Registers native types to be available in the Lua environment.
         */
        void RegisterTypes();
    };
} // namespace Vakol