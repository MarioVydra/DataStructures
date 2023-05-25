#pragma once
#include <libds/heap_monitor.h>
#include <functional>

template <typename DataType, typename Iterator>
class SortingAlgorithm
{
public:
	void sort(std::function<bool(DataType&, DataType&)> comparator, Iterator begin, Iterator end)
	{
		for (Iterator i = begin; i != end; ++i)
		{
			for (Iterator j = i; ++j != end;)
			{
				if (comparator(*i, *j))
				{
					std::swap(*i, *j);
				}
			}
		}
	};
};