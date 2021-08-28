#pragma once
#include <mutex>

class Logger
{
public:
	Logger();

	void LogInfo(const char* format, ...);
	void LogWarning(const char* format, ...);
	void LogError(const char* format, ...);
	void LogDebug(const char* format, ...);

private:
	void pushLog(const char * badge, const char* format, va_list ap);
	std::mutex _mutexLock;
};