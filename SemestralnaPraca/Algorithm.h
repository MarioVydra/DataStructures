#pragma once
#include <libds/heap_monitor.h>
#include <functional>

template <typename DataType, typename Iterator>
class Algorithm
{
public:
	void filter(std::function<bool(DataType&)> predicate, std::function<void(DataType&)> outputFunction, Iterator begin, Iterator end)
	{
		while (begin != end)
		{
			if (predicate(*(begin)))
			{
				outputFunction(*(begin));
			}
			++begin;
		}
	};
};
