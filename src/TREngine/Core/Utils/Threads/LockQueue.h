#pragma once
#include <mutex>
#include <queue>
#include <thread>
#include <functional>

struct LockQueue
{
	using func = std::function<void()>;
	std::mutex Mutex;
	std::condition_variable Condition;
	bool ShouldEnd = false;
	std::queue<func> Tasks;
};