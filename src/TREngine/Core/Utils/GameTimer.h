#pragma once
#include <glm/glm.hpp>
#include <chrono>

using namespace std::chrono;
class GameTimer
{

public:
	GameTimer();

	double GetTimeFromGameStartInSeconds() const;
private:
	steady_clock::time_point _startTime;
};