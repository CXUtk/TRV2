#include "FixedThreadPool.h"

FixedThreadPool::FixedThreadPool(size_t size)
{
	for (int i = 0; i < size; i++)
	{
		std::thread([this]() {
			std::unique_lock<std::mutex> lock(_queue.Mutex);
			while (true)
			{
				if (!_queue.Tasks.empty())
				{
					auto curTask = _queue.Tasks.front();
					_queue.Tasks.pop();
					lock.unlock();

					curTask();

					lock.lock();
				}
				else if (_queue.ShouldEnd)
				{
					break;
				}
				else
				{
					_queue.Condition.wait(lock);
				}
			}
		}).detach();
	}
}

FixedThreadPool::~FixedThreadPool()
{
	{
		std::lock_guard<std::mutex> lock(_queue.Mutex);
		_queue.ShouldEnd = true;
	}
	_queue.Condition.notify_all();
}

void FixedThreadPool::RunAsync(LockQueue::func func)
{
	{
		std::lock_guard<std::mutex> lock(_queue.Mutex);
		_queue.Tasks.push(func);
	}
	_queue.Condition.notify_one();
}
