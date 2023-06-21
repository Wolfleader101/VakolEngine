#include "ScriptEngine.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "Controller/Logger.hpp"

#define DUK_STRING_PUSH_SAFE (1 << 0) /* no error if file does not exist */

namespace Vakol {
    static duk_ret_t native_print(duk_context* ctx) {
        duk_push_string(ctx, " ");
        duk_insert(ctx, 0);
        duk_join(ctx, duk_get_top(ctx) - 1);

        Controller::Logger::ScriptPrintTrace(duk_safe_to_string(ctx, -1));
        return 0;
    }

    static duk_ret_t native_print_info(duk_context* ctx) {
        duk_push_string(ctx, " ");
        duk_insert(ctx, 0);
        duk_join(ctx, duk_get_top(ctx) - 1);

        Controller::Logger::ScriptPrintInfo(duk_safe_to_string(ctx, -1));
        return 0;
    }

    static duk_ret_t native_print_warn(duk_context* ctx) {
        duk_push_string(ctx, " ");
        duk_insert(ctx, 0);
        duk_join(ctx, duk_get_top(ctx) - 1);

        Controller::Logger::ScriptPrintWarn(duk_safe_to_string(ctx, -1));
        return 0;
    }

    static duk_ret_t native_print_err(duk_context* ctx) {
        duk_push_string(ctx, " ");
        duk_insert(ctx, 0);
        duk_join(ctx, duk_get_top(ctx) - 1);

        Controller::Logger::ScriptPrintError(duk_safe_to_string(ctx, -1));
        return 0;
    }

    static duk_ret_t native_print_crit(duk_context* ctx) {
        duk_push_string(ctx, " ");
        duk_insert(ctx, 0);
        duk_join(ctx, duk_get_top(ctx) - 1);

        Controller::Logger::ScriptPrintCrit(duk_safe_to_string(ctx, -1));
        return 0;
    }

    static void fatal_callback(void* udata, const char* msg) {
        (void)udata; /* ignored in this case, silence warning */

        /* Note that 'msg' may be NULL. */
        std::string str = "*** FATAL SCRIPT ERROR: " + std::string((msg ? msg : "no message"));
        VK_CRITICAL(str);
        abort();
    }

    static void duk_push_string_file_raw(duk_context* ctx, const std::string& path, duk_uint_t flags) {
        // Open the file
        std::ifstream file(path, std::ios::binary);
        if (!file) {
            if (flags & DUK_STRING_PUSH_SAFE) {
                duk_push_undefined(ctx);
                return;
            } else {
                duk_error(ctx, DUK_ERR_ERROR, "Could not open file: %s", path.c_str());
                return;  // Control never returns here
            }
        }

        // Read the file into a string
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string file_contents = ss.str();

        if (file.fail()) {
            if (flags & DUK_STRING_PUSH_SAFE) {
                duk_push_undefined(ctx);
                return;
            } else {
                duk_error(ctx, DUK_ERR_ERROR, "Could not read file: %s", path.c_str());
                return;  // Control never returns here
            }
        }

        // Push the file contents onto the Duktape stack as a string
        duk_push_lstring(ctx, file_contents.c_str(), file_contents.size());
    }

    static duk_int_t duk_peval_file(duk_context* ctx, const char* path) {
        duk_int_t rc;

        duk_push_string_file_raw(ctx, path, DUK_STRING_PUSH_SAFE);
        duk_push_string(ctx, path);
        rc = duk_pcompile(ctx, DUK_COMPILE_EVAL);
        if (rc != 0) {
            return rc;  // error
        }
        duk_push_global_object(ctx); /* 'this' binding */
        rc = duk_pcall_method(ctx, 0);
        return rc;  // no error
    }

    ScriptEngine::ScriptEngine() : m_ctx(nullptr) {
        m_ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal_callback);

        if (!m_ctx) {
            VK_CRITICAL("Failed to create Duktape context");
            return;
        }

        this->RegisterFunctions();

        // Evaluate a JavaScript file
        this->RunFile("scripts/test.js");

        std::vector<JSArg> args = {123, "hello", true, 3.14};
        this->RunFunction("testArgs", args);
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

    ScriptEngine::~ScriptEngine() { duk_destroy_heap(m_ctx); }
}  // namespace Vakol