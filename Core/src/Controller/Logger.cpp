#include "Logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Vakol::Controller
{
	std::shared_ptr<spdlog::logger> Logger::s_logger;

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_logger = spdlog::stdout_color_mt("VAKOL");
		s_logger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>& Logger::GetLogger()
	{
		return s_logger;
	}
}