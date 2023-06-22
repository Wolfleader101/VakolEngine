#include "ScriptEngine.hpp"

#include "Controller/Logger.hpp"
#include "DukUtils.hpp"
#include "NativeFunctions.hpp"

namespace Vakol {

    ScriptEngine::ScriptEngine() : m_ctx(nullptr) {
        m_ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal_callback);

        if (!m_ctx) {
            VK_CRITICAL("Failed to create Duktape context");
            return;
        }

        // TODO - register global vars

        this->RegisterFunctions();

        // Evaluate a JavaScript file
        this->RunFile("scripts/test.js");

        CreateScript("scripts/test.js");

        std::vector<JSType> args = {123, "hello", true, 3.14};
        this->RunFunction("testArgs", args);
    }

    ScriptEngine::~ScriptEngine() {
        for (auto ctx : m_scriptCtxs) {
            duk_destroy_heap(ctx);
        }
        duk_destroy_heap(m_ctx);
    }

    JSScript ScriptEngine::CreateScript(const std::string& scriptPath) {
        JSScript script;
        script.env_ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal_callback);
        script.path = scriptPath;

        if (!script.env_ctx) {
            VK_CRITICAL("Failed to create Duktape context");
            return script;
        }

        // TODO -  remove this code, only have it for testing (you probably dont want to run every script on creation?)
        this->RunFile(scriptPath);

        // Store the new context so that we can destroy it later
        //! this might be doubling handling, may not need to store a list of contexts
        m_scriptCtxs.push_back(script.env_ctx);

        return script;
    }

    JSType ScriptEngine::GetGlobal(const std::string& name) { return GetVariable(m_ctx, name); }

    void ScriptEngine::SetGlobal(const std::string& name, const JSType& value) { SetVariable(m_ctx, name, value); }

    JSType ScriptEngine::GetScriptVariable(const JSScript& script, const std::string& varName) {
        return GetVariable(script.env_ctx, varName);
    }

    void ScriptEngine::SetScriptVariable(const JSScript& script, const std::string& name, const JSType& value) {
        SetVariable(script.env_ctx, name, value);
    }

    void ScriptEngine::RunFile(const std::string& file) {
        if (duk_peval_file(m_ctx, file.c_str()) != 0) {
            VK_CRITICAL("Could not evaluate file ({0}): {1}", file, duk_safe_to_string(m_ctx, -1));
            duk_pop(m_ctx);
            return;
        }
        // Clear the result of the script evaluation
        duk_pop(m_ctx);
    }

    void ScriptEngine::RunFunction(const std::string& funcName, const std::vector<JSType>& args) {
        // Get the function
        duk_get_global_string(m_ctx, funcName.c_str());

        // Check if it's callable
        if (!duk_is_callable(m_ctx, -1)) {
            VK_CRITICAL("Function {0} is not callable.", funcName);
            duk_pop(m_ctx);  // Pop non-callable value from the stack
            return;
        }

        // Push arguments onto the stack
        for (const auto& arg : args) {
            PushArg(m_ctx, arg);
        }

        // Call the function with n arguments, and check for errors
        if (duk_pcall(m_ctx, args.size()) != DUK_EXEC_SUCCESS) {
            VK_CRITICAL("Error calling function: {0}", duk_safe_to_string(m_ctx, -1));
        } else {
            VK_INFO("Function returned: {0}", duk_safe_to_string(m_ctx, -1));
        }

        // Pop the result or error from the stack
        duk_pop(m_ctx);
    }

    JSType ScriptEngine::GetVariable(duk_context* ctx, const std::string& varName) {
        // Get the variable
        duk_get_global_string(ctx, varName.c_str());

        // Check the type and get the value
        if (duk_is_string(ctx, -1)) {
            std::string val = duk_get_string(ctx, -1);
            duk_pop(ctx);
            return val;
        } else if (duk_is_number(ctx, -1)) {
            double val = duk_get_number(ctx, -1);
            duk_pop(ctx);
            return val;
        } else if (duk_is_boolean(ctx, -1)) {
            bool val = duk_get_boolean(ctx, -1);
            duk_pop(ctx);
            return val;
        } else if (duk_is_null_or_undefined(ctx, -1)) {
            duk_pop(ctx);
            return std::string();  // Return empty string as null
        } else if (duk_is_object(ctx, -1)) {
            auto obj = std::make_shared<JSObject>();
            duk_enum(ctx, -1, 0); /* Enumerate keys in object on top of stack */
            while (duk_next(ctx, -1, 1 /*get_value*/)) {
                /* -1 is value, -2 is key */
                std::string key = duk_get_string(ctx, -2);
                // Recursively call GetVariable for object's properties.
                JSType val = GetVariable(ctx, key);

                obj->emplace(key, val);
                duk_pop_2(ctx); /* pop key value pair */
            }
            duk_pop(ctx); /* pop enumerator object */
            return obj;
        } else {
            // Unsupported type or an error occurred
            VK_CRITICAL("Unsupported variable type or error occurred while getting variable {0}", varName);
            duk_pop(ctx);
            return std::string();  // Return empty string for error
        }
    }

    void ScriptEngine::SetVariable(duk_context* ctx, const std::string& varName, const JSType& value) {
        // Push variable name onto the stack
        duk_push_string(ctx, varName.c_str());

        // Push value onto the stack
        PushArg(ctx, value);

        // Set the variable in the global object
        duk_put_global_string(ctx, varName.c_str());
    }

    void ScriptEngine::PushArg(duk_context* ctx, const JSType& arg) {
        if (auto pval = std::get_if<std::shared_ptr<JSObject>>(&arg)) {
            duk_push_object(ctx);
            auto& map = **pval;
            for (const auto& [key, value] : map) {
                PushArg(ctx, value);
                duk_put_prop_string(ctx, -2, key.c_str());
            }
        } else {
            std::visit(
                [this, ctx](auto&& val) {
                    using T = std::decay_t<decltype(val)>;
                    if constexpr (std::is_same_v<T, int>)
                        duk_push_int(ctx, val);
                    else if constexpr (std::is_same_v<T, double>)
                        duk_push_number(ctx, val);
                    else if constexpr (std::is_same_v<T, bool>)
                        duk_push_boolean(ctx, val);
                    else if constexpr (std::is_same_v<T, std::string>)
                        duk_push_string(ctx, val.c_str());
                },
                arg);
        }
    }

    void ScriptEngine::RegisterFunctions() {
        //! TODO MOVE THESE INTO SEPERATE FILE
        duk_push_c_function(m_ctx, native_print, DUK_VARARGS);
        duk_put_global_string(m_ctx, "print");

        duk_push_c_function(m_ctx, native_print_info, DUK_VARARGS);
        duk_put_global_string(m_ctx, "print_info");

        duk_push_c_function(m_ctx, native_print_warn, DUK_VARARGS);
        duk_put_global_string(m_ctx, "print_warn");

        duk_push_c_function(m_ctx, native_print_err, DUK_VARARGS);
        duk_put_global_string(m_ctx, "print_err");

        duk_push_c_function(m_ctx, native_print_crit, DUK_VARARGS);
        duk_put_global_string(m_ctx, "print_crit");
    }

}  // namespace Vakol