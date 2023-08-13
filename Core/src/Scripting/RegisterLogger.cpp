#include "Logger/Logger.hpp"
#include "LuaAccess.hpp"

namespace Vakol
{
    void RegisterLogger(sol::state& lua)
    {
        lua.set_function("print", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintTrace(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintTrace(std::to_string(va[0].get<float>()));
            else if (arg.get_type() == sol::type::boolean)
                Logger::ScriptPrintTrace(std::to_string(va[0].get<bool>()));
        });

        lua.set_function("print_info", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintInfo(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintInfo(std::to_string(va[0].get<float>()));
        });

        lua.set_function("print_warn", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintWarn(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintWarn(std::to_string(va[0].get<float>()));
        });

        lua.set_function("print_err", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintError(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintError(std::to_string(va[0].get<float>()));
        });

        lua.set_function("print_critical", [](const sol::variadic_args& va) {
            if (const auto arg = va[0]; arg.get_type() == sol::type::string)
                Logger::ScriptPrintCrit(va[0].get<std::string>());
            else if (arg.get_type() == sol::type::number)
                Logger::ScriptPrintCrit(std::to_string(va[0].get<float>()));
        });
    }
} // namespace Vakol