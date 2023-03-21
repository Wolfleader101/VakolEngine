#pragma once

#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace Vakol::Controller {
    /**
     * @class Logger
     * @brief the logger class
     */
    class Logger {
       public:
        /**
         * @brief initialise the logger
         */
        static void Init();

        /**
         * @brief get the logger
         * @return return pointer to logger
         */
        static std::shared_ptr<spdlog::logger>& GetLogger();

        static void ScriptPrintTrace(std::string args);
        static void ScriptPrintInfo(std::string args);
        static void ScriptPrintWarn(std::string args);
        static void ScriptPrintError(std::string args);
        static void ScriptPrintCrit(std::string args);

       private:
        /**
         * @brief logger pointer
         */
        static std::shared_ptr<spdlog::logger> s_logger;

        /**
         * @brief  logger ptr for lua
         *
         */
        static std::shared_ptr<spdlog::logger> s_script_logger;
    };
}  // namespace Vakol::Controller

#define VK_TRACE(...) ::Vakol::Controller::Logger::GetLogger()->trace(__VA_ARGS__)
#define VK_INFO(...) ::Vakol::Controller::Logger::GetLogger()->info(__VA_ARGS__)
#define VK_WARN(...) ::Vakol::Controller::Logger::GetLogger()->warn(__VA_ARGS__)
#define VK_ERROR(...) ::Vakol::Controller::Logger::GetLogger()->error(__VA_ARGS__)
#define VK_CRITICAL(...) ::Vakol::Controller::Logger::GetLogger()->critical(__VA_ARGS__)