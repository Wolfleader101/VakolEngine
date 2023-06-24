#pragma once

#include <duktape.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Vakol {
    struct JSScript {
        duk_context* env_ctx;
        std::string path;
    };

    struct JSObject;

    using JSType = std::variant<int, double, bool, std::string, std::shared_ptr<JSObject>>;

    struct JSObject : public std::unordered_map<std::string, JSType> {};

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
        ~ScriptEngine();

        // virtual void Enable(){};
        // virtual void Disable(){};

        JSScript CreateScript(const std::string& scriptPath);  //? should this return a ref?

        JSType GetGlobal(const std::string& name);  //? should this be made const ref?
        void SetGlobal(const std::string& name, const JSType& value);

        JSType GetScriptVariable(const JSScript& script,
                                 const std::string& varName);  //? should this be made const ref?
        void SetScriptVariable(const JSScript& script, const std::string& varName, const JSType& value);

        void Update();

        void Tick();  //? should the script engine handle its own tickrate, or should this be called from app?

       private:
        duk_context* m_ctx;  //! the global context
        std::vector<duk_context*> m_scriptCtxs;
        std::vector<JSScript> m_scripts;  //! just for testing - remove later

        void PushArg(duk_context* ctx, const JSType& arg);
        JSType GetVariable(duk_context* ctx, const std::string& varName);
        void SetVariable(duk_context* ctx, const std::string& varName, const JSType& value);

        void RunFile(duk_context* ctx, const std::string& file);
        void RunFunction(duk_context* ctx, const std::string& funcName,
                         const std::vector<JSType>& args);  //! this runs using global context - probably not ideal

        void RegisterFunctions(duk_context* ctx);
        void RegisterVars(duk_context* ctx);
    };

}  // namespace Vakol