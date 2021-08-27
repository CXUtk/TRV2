#pragma once

class Logger
{
public:
	Logger();

	void LogInfo(const char* format, ...);
	void LogWarning(const char* format, ...);
	void LogError(const char* format, ...);
	void LogDebug(const char* format, ...);
};