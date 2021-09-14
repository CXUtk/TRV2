#include "GameTimer.h"
TRV2_NAMESPACE_BEGIN
GameTimer::GameTimer()
{
	_startTime = steady_clock::now();
}

double GameTimer::GetTimeFromGameStartInSeconds() const
{
	return duration_cast<duration<double>>(steady_clock::now() - _startTime).count();
}
TRV2_NAMESPACE_END