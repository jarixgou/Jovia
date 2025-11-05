#include "Logger.hpp"

#include <iomanip>

#define RESET   "\033[0m"

Engine::LogLevel Engine::Logger::m_minimumLevel = LogLevel::DEBUG;
std::vector<std::string> Engine::Logger::m_logHistory;

std::string Engine::Logger::GetLevelPrefix(LogLevel _level)
{
	switch (_level)
	{
	case LogLevel::DEBUG:			return "[DEBUG]";
	case LogLevel::INFO:			return "[INFO]";
	case LogLevel::WARNING:			return "[WARNING]";
	case LogLevel::LOG_ERROR:		return "[ERROR]";
	case LogLevel::CRITICAL:		return "[CRITICAL]";
	default:						return "[UNKNOWN]";
	}
}

std::string Engine::Logger::GetLevelColor(LogLevel _level)
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

void Engine::Logger::Log(LogLevel _level, const char* _message)
{
	if (_level < m_minimumLevel)
	{
		return;	
	}

	auto now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
	std::tm localTime;
	localtime_s(&localTime, &currentTime);

	std::string color = GetLevelColor(_level);
	std::string prefix = GetLevelPrefix(_level);

	std::ostringstream oss;
	oss << color << "[" << std::put_time(&localTime, "%H:%M:%S") << "] "
		<< prefix << " - " << _message << RESET << "\n";
	std::string outPut = oss.str();
	
	std::cout << outPut;

	std::ostringstream logFile;
	logFile << "[" << std::put_time(&localTime, "%H:%M:%S") << "] " << prefix << " - " << _message << "\n";
	outPut = logFile.str();
	m_logHistory.push_back(outPut);
}

void Engine::Logger::SetMinimumLogLevel(LogLevel _level)
{
	m_minimumLevel = _level;
}

const std::vector<std::string>& Engine::Logger::GetLogHistory()
{
	return m_logHistory;
}