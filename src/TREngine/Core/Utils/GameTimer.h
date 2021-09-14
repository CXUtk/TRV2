#pragma once
#include <glm/glm.hpp>
#include <chrono>
#include <Core.h>

using namespace std::chrono;
TRV2_NAMESPACE_BEGIN
class GameTimer
{

public:
	GameTimer();

	double GetTimeFromGameStartInSeconds() const;
private:
	steady_clock::time_point _startTime;
};
TRV2_NAMESPACE_END