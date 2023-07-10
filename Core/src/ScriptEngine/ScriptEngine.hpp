#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <string>
#include <vector>

namespace Vakol {

    // prefixing with Lua for now to avoid name collisions
    struct LuaScript {
        sol::environment env;
        std::string path;
    };

    //! if we dont want to expose sol at all we could do something like this
    // struct JSObject;

    // using LuaType = std::variant<int, double, bool, std::string, std::shared_ptr<JSObject>>;

    // struct JSObject : public std::unordered_map<std::string, LuaType> {};

    using LuaType = sol::object;

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

        LuaScript CreateScript(const std::string& scriptPath);

        LuaType GetGlobalVariable(const std::string& name);

        template <typename T>
        void SetGlobalVariable(const std::string& name, T&& value) {
            m_state[name] = std::forward<T>(value);
        }

        LuaType GetScriptVariable(const LuaScript& script, const std::string& varName);

        template <typename T>
        void SetScriptVariable(LuaScript& script, const std::string& varName, T&& value) {
            script.env[varName] = std::forward<T>(value);

            m_state.set_function("app_run", &Application::SetRunning, app);
        }

        template <typename... Args, typename Key>
        void SetGlobalFunction(Key&& key, Args&&... args) {
            m_state.set_function(std::forward<Key>(key), std::forward<Args>(args)...);
        }

        template <typename... Args, typename Key>
        void SetScriptFunction(LuaScript& script, Key&& key, Args&&... args) {
            script.env.set_function(std::forward<Key>(key), std::forward<Args>(args)...);
        }

        void Update(LuaScript& script);

        void Tick(LuaScript& script);

       private:
        //! global state
        sol::state m_state;

        LuaType GetVariable(sol::environment env, const std::string& varName);

        void RunFile(sol::environment env, const std::string& file);
        void RunFunction(sol::environment env, const std::string& funcName, bool error = true,
                         const std::vector<LuaType>& args = {});

        void RegisterFunctions();
        void RegisterTypes();
    };
}  // namespace Vakol