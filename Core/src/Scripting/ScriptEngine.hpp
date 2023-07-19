#pragma once

#include <string>
#include <vector>

#include "ScriptTypes.hpp"

namespace Vakol {

    // class EngineSystem {  // interface for engine system, something to think about
    //    public:
    //     virtual void Enable() = 0;
    //     virtual void Disable() = 0;
    //     virtual void Init() = 0;  //? could take in registry shared ptr and time?
    //     virtual void Update() = 0;
    //     virtual void Tick() = 0;
    // };

    class ScriptEngine {
       public:
        ScriptEngine();

        // WARNING - lua state global access deprecated
        LuaScript CreateScript(const std::string& scriptPath);

        LuaTable CreateTable();

        LuaType GetGlobalVariable(const std::string& name);

        template <typename T>
        void SetGlobalVariable(const std::string& name, T&& value) {
            m_state[name] = std::forward<T>(value);
        }

        LuaType GetScriptVariable(const LuaScript& script, const std::string& varName);

        template <typename T>
        void SetScriptVariable(LuaScript& script, const std::string& varName, T&& value) {
            script.env[varName] = std::forward<T>(value);
        }

        template <typename... Args, typename Key>
        void SetGlobalFunction(Key&& key, Args&&... args) {
            m_state.set_function(std::forward<Key>(key), std::forward<Args>(args)...);
        }

        template <typename... Args, typename Key>
        void SetScriptFunction(LuaScript& script, Key&& key, Args&&... args) {
            script.env.set_function(std::forward<Key>(key), std::forward<Args>(args)...);
        }

        void InitScript(LuaScript& script);

        void UpdateScript(LuaScript& script);

        void TickScript(LuaScript& script);

       private:
        //! global state
        LuaState m_state;

        LuaType GetVariable(sol::environment env, const std::string& varName);

        void RunFile(sol::environment env, const std::string& file);
        void RunFunction(sol::environment env, const std::string& funcName, bool ignoreMissing = true,
                         const std::vector<LuaType>& args = {});

        void RegisterFunctions();
        void RegisterTypes();
    };
}  // namespace Vakol