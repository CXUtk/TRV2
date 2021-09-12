#pragma once
#include <mutex>
#include <Core.h>

TRV2_NAMESPACE_BEGIN

enum class SeverityLevel
{
	Info,
	Warning,
	Error,
	Debug,

	__COUNT
};

class Logger
{
public:
	Logger();

	void Log(SeverityLevel slevel, const char* format, ...) const;
	//void LogWarning(const char* format, ...) const;
	//void LogError(const char* format, ...) const;
	//void LogDebug(const char* format, ...) const;

private:
	void pushLog(SeverityLevel slevel, const char * badge, const char* format, va_list ap) const;
	mutable std::mutex _mutexLock;
};
TRV2_NAMESPACE_END