#ifndef SORTER_H
#define SORTER_H

#include <algorithm>

template<typename DataType> class StandartSorter
{
	public:
		template<typename RandomAccessIterator, typename Comparator>
		inline void operator () (RandomAccessIterator begin, RandomAccessIterator end, Comparator comparator)
		{
			std::sort(begin, end, comparator);
		}
};

#endif
