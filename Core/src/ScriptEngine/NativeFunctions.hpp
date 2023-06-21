#pragma once

#include <duktape.h>

namespace Vakol {
    duk_ret_t native_print(duk_context* ctx) {
        duk_push_string(ctx, " ");
        duk_insert(ctx, 0);
        duk_join(ctx, duk_get_top(ctx) - 1);

        Controller::Logger::ScriptPrintTrace(duk_safe_to_string(ctx, -1));
        return 0;
    }

    duk_ret_t native_print_info(duk_context* ctx) {
        duk_push_string(ctx, " ");
        duk_insert(ctx, 0);
        duk_join(ctx, duk_get_top(ctx) - 1);

        Controller::Logger::ScriptPrintInfo(duk_safe_to_string(ctx, -1));
        return 0;
    }

    duk_ret_t native_print_warn(duk_context* ctx) {
        duk_push_string(ctx, " ");
        duk_insert(ctx, 0);
        duk_join(ctx, duk_get_top(ctx) - 1);

        Controller::Logger::ScriptPrintWarn(duk_safe_to_string(ctx, -1));
        return 0;
    }

    duk_ret_t native_print_err(duk_context* ctx) {
        duk_push_string(ctx, " ");
        duk_insert(ctx, 0);
        duk_join(ctx, duk_get_top(ctx) - 1);

        Controller::Logger::ScriptPrintError(duk_safe_to_string(ctx, -1));
        return 0;
    }

    duk_ret_t native_print_crit(duk_context* ctx) {
        duk_push_string(ctx, " ");
        duk_insert(ctx, 0);
        duk_join(ctx, duk_get_top(ctx) - 1);

        Controller::Logger::ScriptPrintCrit(duk_safe_to_string(ctx, -1));
        return 0;
    }
}  // namespace Vakol