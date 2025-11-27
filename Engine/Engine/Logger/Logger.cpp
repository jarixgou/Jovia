#include "Logger.hpp"

#include <fstream>
#include <iomanip>

#define RESET   "\033[0m"

namespace Engine

{
	LogLevel Logger::m_minimumLevel = LogLevel::DEBUG;
	std::vector<std::string> Logger::m_logHistory;

	std::string Logger::GetLevelPrefix(LogLevel _level)
	{
		switch (_level)
		{
		case LogLevel::DEBUG:			return GetLevelColor(_level) + "[DEBUG]" + RESET;
		case LogLevel::INFO:			return GetLevelColor(_level) + "[INFO]" + RESET;
		case LogLevel::WARNING:			return GetLevelColor(_level) + "[WARNING]" + RESET;
		case LogLevel::LOG_ERROR:		return GetLevelColor(_level) + "[ERROR]" + RESET;
		case LogLevel::CRITICAL:		return GetLevelColor(_level) + "[CRITICAL]" + RESET;
		default:						return GetLevelColor(_level) + "[UNKNOWN]" + RESET;
		}
	}

	std::string Logger::GetLevelColor(LogLevel _level)
	{
		switch (_level)
		{
		case LogLevel::DEBUG:		return CYAN;
		case LogLevel::INFO:		return GREEN;
		case LogLevel::WARNING:		return YELLOW;
		case LogLevel::LOG_ERROR:	return RED;
		case LogLevel::CRITICAL:	return PURPLE;
		default:					return WHITE;
		}
	}

	void Logger::Log(LogLevel _level, const char* _message, bool _fromEngine)
	{
		if (_level < m_minimumLevel)
		{
			return;
		}

		std::string prefixName = "";
		if (_fromEngine)
		{
			prefixName = LIGHT_BLUE + std::string("[ENGINE]") + RESET;
		}
		else
		{
			prefixName = LIGHT_PURPLE + std::string("[SCRIPT]") + RESET;
		}

		auto now = std::chrono::system_clock::now();
		std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
		std::tm localTime;
		localtime_s(&localTime, &currentTime);

		std::string prefix = GetLevelPrefix(_level);

		std::ostringstream oss;
		oss << "[" << std::put_time(&localTime, "%H:%M:%S") << "] " << prefixName << " "
			<< prefix << " - " << _message << RESET << "\n";
		std::string outPut = oss.str();

		std::cout << outPut;

		std::ostringstream logFile;
		logFile << "[" << std::put_time(&localTime, "%H:%M:%S") << "] " << prefix << " - " << _message << "\n";
		outPut = logFile.str();
		m_logHistory.push_back(outPut);

		if (_level == LogLevel::CRITICAL)
		{
			WriteLogFile();
			exit(-1);
		}
	}

	void Logger::SetMinimumLogLevel(LogLevel _level)
	{
		m_minimumLevel = _level;
	}

	const std::vector<std::string>& Logger::GetLogHistory()
	{
		return m_logHistory;
	}

	void Logger::WriteLogFile()
	{
		std::ofstream logFile("log.txt");

		if (!logFile.is_open())
		{
			LOG_ERROR("can't open file", true);
			return;
		}

		for (const auto& entry : m_logHistory)
		{
			logFile << entry;
		}
	}

	void Logger::Clear()
	{
		m_logHistory.clear();
	}
}
