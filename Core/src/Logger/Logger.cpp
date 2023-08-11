#include "Logger/Logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Vakol
{
    std::shared_ptr<spdlog::logger> Logger::s_logger;
    std::shared_ptr<spdlog::logger> Logger::s_script_logger;

    void Logger::Init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_logger = spdlog::stdout_color_mt("VAKOL");
        s_logger->set_level(spdlog::level::trace);

        s_script_logger = spdlog::stdout_color_mt("SCRIPT");
        s_script_logger->set_pattern("%^[%T]    >> %n: %v%$");
        s_script_logger->set_level(spdlog::level::trace);
    }

    std::shared_ptr<spdlog::logger>& Logger::GetLogger()
    {
        return s_logger;
    }

    void Logger::ScriptPrintTrace(std::string args)
    {
        s_script_logger->trace(args);
    }
    void Logger::ScriptPrintInfo(std::string args)
    {
        s_script_logger->info(args);
    }
    void Logger::ScriptPrintWarn(std::string args)
    {
        s_script_logger->warn(args);
    }
    void Logger::ScriptPrintError(std::string args)
    {
        s_script_logger->error(args);
    }
    void Logger::ScriptPrintCrit(std::string args)
    {
        s_script_logger->critical(args);
    }
} // namespace Vakol