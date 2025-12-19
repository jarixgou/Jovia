#ifndef LOGGER__HPP
#define LOGGER__HPP

#include <string>
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <vector>

namespace Engine
{
	// ANSI color code definitions for console output
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

	/**
	 * @enum LogLevel
	 * @brief Severity levels for log messages
	 */
	enum class LogLevel
	{
		DEBUG,      ///< Detailed debug information
		INFO,       ///< General informational messages
		WARNING,    ///< Warning messages for potential issues
		LOG_ERROR,  ///< Error messages for failures
		CRITICAL    ///< Critical errors requiring immediate attention
	};

	/**
	 * @class Logger
	 * @brief Centralized logging system with colored console output
	 * 
	 * Provides structured logging with different severity levels, colored output,
	 * log history tracking, and file export capabilities. Distinguishes between
	 * engine and game logs.
	 */
	class Logger
	{
	private:
		static LogLevel m_minimumLevel;             ///< Minimum level to display
		static std::vector<std::string> m_logHistory; ///< History of all logged messages

		/**
		 * @brief Gets the text prefix for a log level
		 * 
		 * @param _level Log severity level
		 * @return String prefix (e.g., "[DEBUG]", "[ERROR]")
		 */
		static std::string GetLevelPrefix(LogLevel _level);

		/**
		 * @brief Gets the ANSI color code for a log level
		 * 
		 * @param _level Log severity level
		 * @return ANSI color code string
		 */
		static std::string GetLevelColor(LogLevel _level);
	public:
		/**
		 * @brief Logs a message with specified severity
		 * 
		 * Outputs a timestamped, colored message to the console and stores it
		 * in the log history. Messages below the minimum level are filtered out.
		 * 
		 * @param _level Severity level of the message
		 * @param _message Text content of the log
		 * @param _fromEngine True if message originates from engine, false if from game
		 */
		static void Log(LogLevel _level, const char* _message, bool _fromEngine);

		/**
		 * @brief Sets the minimum log level to display
		 * 
		 * Messages below this level will be filtered out.
		 * 
		 * @param _level Minimum severity level to display
		 */
		static void SetMinimumLogLevel(LogLevel _level);

		/**
		 * @brief Gets the complete log history
		 * 
		 * @return Const reference to vector of all logged messages
		 */
		static const std::vector<std::string>& GetLogHistory();

		/**
		 * @brief Exports the log history to a file
		 * 
		 * Writes all logged messages to a timestamped log file.
		 */
		static void WriteLogFile();

		/**
		 * @brief Clears the log history
		 */
		static void Clear();
	};
}

/**
 * @brief Convenience macro for debug-level logging
 * @param text Message to log
 * @param engine True if from engine, false if from game
 */
#define LOG_DEBUG(text, engine)    Engine::Logger::Log(Engine::LogLevel::DEBUG, text, engine)

/**
 * @brief Convenience macro for info-level logging
 * @param text Message to log
 * @param engine True if from engine, false if from game
 */
#define LOG_INFO(text, engine)     Engine::Logger::Log(Engine::LogLevel::INFO, text, engine)

/**
 * @brief Convenience macro for warning-level logging
 * @param text Message to log
 * @param engine True if from engine, false if from game
 */
#define LOG_WARNING(text, engine)  Engine::Logger::Log(Engine::LogLevel::WARNING, text, engine)

/**
 * @brief Convenience macro for error-level logging
 * @param text Message to log
 * @param engine True if from engine, false if from game
 */
#define LOG_ERROR(text, engine)    Engine::Logger::Log(Engine::LogLevel::LOG_ERROR, text, engine)

/**
 * @brief Convenience macro for critical-level logging
 * @param text Message to log
 * @param engine True if from engine, false if from game
 */
#define LOG_CRITICAL(text, engine) Engine::Logger::Log(Engine::LogLevel::CRITICAL, text, engine)

#endif // !LOGGER__HPP