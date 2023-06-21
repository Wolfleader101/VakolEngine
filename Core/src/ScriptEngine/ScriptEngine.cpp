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

        std::vector<JSArg> args = {123, "hello", true, 3.14};
        this->RunFunction("testArgs", args);
    }

    ScriptEngine::~ScriptEngine() {
        for (auto ctx : m_scriptCtxs) {
            duk_destroy_heap(ctx);
        }
        duk_destroy_heap(m_ctx);
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

    SScript ScriptEngine::CreateScript(const std::string& scriptPath) {
        SScript script;
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

    void ScriptEngine::RunFunction(const std::string& funcName, const std::vector<JSArg>& args) {
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
            std::visit(
                [&](auto&& val) {
                    using T = std::decay_t<decltype(val)>;
                    if constexpr (std::is_same_v<T, int>)
                        duk_push_int(m_ctx, val);
                    else if constexpr (std::is_same_v<T, double>)
                        duk_push_number(m_ctx, val);
                    else if constexpr (std::is_same_v<T, bool>)
                        duk_push_boolean(m_ctx, val);
                    else if constexpr (std::is_same_v<T, std::string>)
                        duk_push_string(m_ctx, val.c_str());
                },
                arg);
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