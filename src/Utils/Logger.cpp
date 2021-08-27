#include "Logger.h"
#include <cstdio>
#include <cstdarg>

static const char* infoBadge = "[INFO]";
static const char* warningBadge = "[WARN]";
static const char* errorBadge = "[ERROR]";
static const char* debugBadge = "[DEBUG]";

Logger::Logger()
{
}

void Logger::LogInfo(const char* format, ...)
{
	fprintf(stdout, "%s ", infoBadge);

	va_list ap;
	va_start(ap, format);
	vfprintf(stdout, format, ap);
	va_end(ap);

	fprintf(stdout, "\n");
}

void Logger::LogWarning(const char* format, ...)
{
}

void Logger::LogError(const char* format, ...)
{
}

void Logger::LogDebug(const char* format, ...)
{
#ifdef _DEBUG
#endif // _DEBUG
}
