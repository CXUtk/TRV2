#pragma once

#include <vector>
#include <functional>

template<typename ... EventArgs>
class Event
{
public:
	using func_type = std::function<void(EventArgs ...args)>;

	Event() {}
	~Event() {}

	// Not allowed to return *self
	void operator+=(const func_type& handler)
	{
		_eventHandlers.push_back(std::move(handler));
	}

	// Not allowed to return *self
	void operator-=(const func_type& handler)
	{
		std::remove(_eventHandlers.begin(), _eventHandlers.end(), handler);
	}

	void Invoke(EventArgs ... args)
	{
		for (auto& handler : _eventHandlers)
		{
			handler(args...);
		}
	}

private:
	std::vector<func_type> _eventHandlers;
};