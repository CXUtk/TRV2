#include "Logger.h"

#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <array>

TRV2_NAMESPACE_BEGIN


template<size_t T>
constexpr std::array<const char*, T> generateBadgeTable()
{
	std::array<const char*, T> M{ 0 };
	M[(int)SeverityLevel::Info] = "[INFO]";
	M[(int)SeverityLevel::Warning] = "[WARNING]";
	M[(int)SeverityLevel::Error] = "[ERROR]";
	M[(int)SeverityLevel::Debug] = "[DEBUG]";
	return M;
}


static constexpr auto badgeTable = generateBadgeTable<(size_t)SeverityLevel::__COUNT>();

void Logger::Log(SeverityLevel slevel, const char* format, ...) const
{
	if (slevel == SeverityLevel::Debug)
	{
#ifdef _DEBUG
		va_list ap;
		va_start(ap, format);
		pushLog(slevel, badgeTable[(int)slevel], format, ap);
		va_end(ap);
#endif
	}
	else
	{
		va_list ap;
		va_start(ap, format);
		pushLog(slevel, badgeTable[(int)slevel], format, ap);
		va_end(ap);
	}
}

Logger::Logger()
{
}
void Logger::pushLog(SeverityLevel slevel, const char* badge, const char* format, va_list ap) const
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