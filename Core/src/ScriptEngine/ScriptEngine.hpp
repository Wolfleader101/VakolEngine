#pragma once

#include <duktape.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "Controller/Time.hpp"

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
        ScriptEngine(Controller::Time& time);
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

        Controller::Time& m_time;  // ! testing

        void PushArg(duk_context* ctx, const JSType& arg);
        JSType GetVariable(duk_context* ctx, const std::string& varName);
        void SetVariable(duk_context* ctx, const std::string& varName, const JSType& value);

        void RunFile(duk_context* ctx, const std::string& file);
        void RunFunction(duk_context* ctx, const std::string& funcName,
                         const std::vector<JSType>& args);  //! this runs using global context - probably not ideal

        void RegisterFunctions(duk_context* ctx);
        void RegisterVars(duk_context* ctx);

        template <typename T>
        void SetGlobal(const std::string& name, T* value) {
            duk_idx_t objIdx = duk_push_object(m_ctx);
            duk_push_pointer(m_ctx, static_cast<void*>(value));
            // Assume objIdx is at -2, pointer is at -1
            duk_put_prop_string(m_ctx, objIdx, "ptr");  // Associate pointer with Duktape object
            duk_put_global_string(m_ctx, name.c_str());
        }
    };

    class DukType {
       public:
        DukType(duk_context* ctx, const std::string& typeName) : m_ctx(ctx), m_typeName(typeName) {
            m_objIdx = duk_push_object(m_ctx);
        }

        template <typename T, typename V>
        void SetProperty(const std::string& propName, V T::*value) {
            duk_push_c_function(m_ctx, getProp<T, V>, 2);
            void* buffer = duk_push_fixed_buffer(m_ctx, sizeof(value));
            std::memcpy(buffer, &value, sizeof(value));
            duk_put_prop_string(m_ctx, -2, propName.c_str());
        }

        ~DukType() { duk_put_global_string(m_ctx, m_typeName.c_str()); }

       private:
        duk_context* m_ctx;
        std::string m_typeName;
        duk_idx_t m_objIdx;

        template <typename T, typename V>
        static duk_ret_t getProp(duk_context* ctx) {
            auto ptr = static_cast<T*>(duk_to_pointer(ctx, 0));
            V T::*memberPtr;
            std::memcpy(&memberPtr, duk_get_buffer_data(ctx, -1, nullptr), sizeof(memberPtr));

            if constexpr (std::is_same<V, std::string>::value) {
                duk_push_string(ctx, (ptr->*memberPtr).c_str());
            } else {
                duk_push_number(ctx, ptr->*memberPtr);
            }
            return 1;
        }
    };

}  // namespace Vakol