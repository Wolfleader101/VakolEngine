#include "ScriptEngine.hpp"

#include <iostream>

#include "Controller/Logger.hpp"

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

    ScriptEngine::ScriptEngine() : m_ctx(nullptr) {
        m_ctx = duk_create_heap_default();

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

        duk_eval_string(m_ctx, "print('Hello world!');");
        duk_eval_string(m_ctx, "print_info('Hello world!');");
        duk_eval_string(m_ctx, "print_warn('Hello world!');");
        duk_eval_string(m_ctx, "print_err('Hello world!');");
        duk_eval_string(m_ctx, "print_crit('Hello world!');");
    }

    ScriptEngine::~ScriptEngine() {
        duk_pop(m_ctx); /* pop eval result */
        duk_destroy_heap(m_ctx);
    }
}  // namespace Vakol