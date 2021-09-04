﻿#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

#include "Logger.h"

TRV2_NAMESPACE_BEGIN
static const char* infoBadge = "[INFO]";
static const char* warningBadge = "[WARN]";
static const char* errorBadge = "[ERROR]";
static const char* debugBadge = "[DEBUG]";

Logger::Logger()
{
}

void Logger::LogInfo(const char* format, ...) const
{
	va_list ap;
	va_start(ap, format);
	pushLog(infoBadge, format, ap);
	va_end(ap);
}

void Logger::LogWarning(const char* format, ...) const
{
	va_list ap;
	va_start(ap, format);
	pushLog(warningBadge, format, ap);
	va_end(ap);
}

void Logger::LogError(const char* format, ...) const
{
	va_list ap;
	va_start(ap, format);
	pushLog(errorBadge, format, ap);
	va_end(ap);
}

void Logger::LogDebug(const char* format, ...) const
{
#ifdef _DEBUG
	va_list ap;
	va_start(ap, format);
	pushLog(debugBadge, format, ap);
	va_end(ap);
#endif // _DEBUG
}

void Logger::pushLog(const char* badge, const char* format, va_list ap) const
{
	const std::lock_guard<std::mutex> lockGuard(_mutexLock);

	// Print badge
	fprintf(stdout, "%s", badge);

	// Print time
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");
	fprintf(stdout, "[%s] ", oss.str().c_str());

	// Print content
	vfprintf(stdout, format, ap);

	fprintf(stdout, "\n");
}
TRV2_NAMESPACE_END