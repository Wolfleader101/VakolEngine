#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Vakol::Controller
{
	/**
	 * @class Logger
	 * @brief the logger class
	*/
	class Logger
	{
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

	private:
		/**
		 * @brief logger pointer
		*/
		static std::shared_ptr<spdlog::logger> s_logger;
	};
}

#define VK_TRACE(...)   ::Vakol::Controller::Logger::GetLogger()->trace(__VA_ARGS__)
#define VK_INFO(...)    ::Vakol::Controller::Logger::GetLogger()->info(__VA_ARGS__)
#define VK_WARN(...)    ::Vakol::Controller::Logger::GetLogger()->warn(__VA_ARGS__)
#define VK_ERROR(...)   ::Vakol::Controller::Logger::GetLogger()->error(__VA_ARGS__)
#define VK_CRITICAL(...)   ::Vakol::Controller::Logger::GetLogger()->critical(__VA_ARGS__)