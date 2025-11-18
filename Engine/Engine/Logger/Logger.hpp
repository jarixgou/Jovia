#ifndef LOGGER__HPP
#define LOGGER__HPP

#include <string>
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <vector>

namespace Engine
{
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE  "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

	enum class LogLevel
	{
		DEBUG,
		INFO,
		WARNING,
		LOG_ERROR,
		CRITICAL
	};

	class Logger
	{
	private:
		static LogLevel m_minimumLevel;
		static std::vector<std::string> m_logHistory;

		static std::string GetLevelPrefix(LogLevel _level);
		static std::string GetLevelColor(LogLevel _level);
	public:
		static void Log(LogLevel _level, const char* _message);
		static void SetMinimumLogLevel(LogLevel _level);
		static const std::vector<std::string>& GetLogHistory();
		static void WriteLogFile();
		static void Clear();
	};
}

#define LOG_DEBUG(text)    Engine::Logger::Log(Engine::LogLevel::DEBUG, text)
#define LOG_INFO(text)     Engine::Logger::Log(Engine::LogLevel::INFO, text)
#define LOG_WARNING(text)  Engine::Logger::Log(Engine::LogLevel::WARNING, text)
#define LOG_ERROR(text)    Engine::Logger::Log(Engine::LogLevel::LOG_ERROR, text)
#define LOG_CRITICAL(text) Engine::Logger::Log(Engine::LogLevel::CRITICAL, text)

#endif // !LOGGER__HPP