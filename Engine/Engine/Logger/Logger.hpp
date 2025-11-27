#ifndef LOGGER__HPP
#define LOGGER__HPP

#include <string>
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <vector>

namespace Engine
{
#define RED				"\033[31m"
#define GREEN			"\033[32m"
#define YELLOW			"\033[33m"
#define BLUE			"\033[34m"
#define PURPLE			"\033[35m"
#define CYAN			"\033[36m"
#define WHITE			"\033[37m"
#define DARK_BLUE		"\033[38;2;0;0;139m"
#define	LIGHT_GRAY		"\033[0;37m"
#define	LIGHT_RED		"\033[1;31m"
#define LIGHT_PURPLE	"\033[1;35m"
#define	LIGHT_BLUE		"\033[1;34m"

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
		static void Log(LogLevel _level, const char* _message, bool _fromEngine);
		static void SetMinimumLogLevel(LogLevel _level);
		static const std::vector<std::string>& GetLogHistory();
		static void WriteLogFile();
		static void Clear();
	};
}

#define LOG_DEBUG(text, engine)    Engine::Logger::Log(Engine::LogLevel::DEBUG, text, engine)
#define LOG_INFO(text, engine)     Engine::Logger::Log(Engine::LogLevel::INFO, text, engine)
#define LOG_WARNING(text, engine)  Engine::Logger::Log(Engine::LogLevel::WARNING, text, engine)
#define LOG_ERROR(text, engine)    Engine::Logger::Log(Engine::LogLevel::LOG_ERROR, text, engine)
#define LOG_CRITICAL(text, engine) Engine::Logger::Log(Engine::LogLevel::CRITICAL, text, engine)

#endif // !LOGGER__HPP