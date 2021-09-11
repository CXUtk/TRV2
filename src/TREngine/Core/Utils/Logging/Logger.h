#pragma once
#include <mutex>
#include <Core.h>

TRV2_NAMESPACE_BEGIN
class Logger
{
public:
	Logger();

	void LogInfo(const char* format, ...) const;
	void LogWarning(const char* format, ...) const;
	void LogError(const char* format, ...) const;
	void LogDebug(const char* format, ...) const;

private:
	void pushLog(const char * badge, const char* format, va_list ap) const;
	mutable std::mutex _mutexLock;
};
TRV2_NAMESPACE_END