#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <string>
#include <unordered_map>
#include <variant>

#include "Controller/Time.hpp"

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
        ScriptEngine(Controller::Time& time);
        ~ScriptEngine();

        LuaScript CreateScript(const std::string& scriptPath);

        LuaType GetGlobal(const std::string& name);
        void SetGlobal(const std::string& name, const LuaType& value);

        LuaType GetScriptVariable(const LuaScript& script, const std::string& varName);
        void SetScriptVariable(LuaScript& script, const std::string& varName, const LuaType& value);

        template <typename Func>
        void RegisterGlobalFunction(const std::string& funcName, Func funcPtr) {
            m_state.set_function(
                funcName, sol::overload([=](sol::variadic_args args) { return sol::call_status::ok, funcPtr(args); }));
        }

        template <typename Func>
        void RegisterScriptFunction(LuaScript& script, const std::string& funcName, Func funcPtr) {
            script.env.set_function(
                funcName, sol::overload([=](sol::variadic_args args) { return sol::call_status::ok, funcPtr(args); }));
        }

        void Update();

        void Tick();

       private:
        std::vector<LuaScript> m_scripts;  //! just for testing - remove later

        //! global state
        sol::state m_state;

        Controller::Time& m_time;  // ! testing

        LuaType GetVariable(sol::environment env, const std::string& varName);

        void RunFile(sol::environment env, const std::string& file);
        void RunFunction(sol::environment env, const std::string& funcName, const std::vector<LuaType>& args);

        void RegisterFunctions();
        void RegisterVars();
    };
}  // namespace Vakol