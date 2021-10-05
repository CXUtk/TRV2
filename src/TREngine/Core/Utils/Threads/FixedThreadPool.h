#pragma once
#include "LockQueue.h"

class FixedThreadPool
{
public:
	FixedThreadPool(size_t size);
	~FixedThreadPool();

	void RunAsync(LockQueue::func func);

private:
	LockQueue _queue;
};