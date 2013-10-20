#ifndef STANDARTSTABLESORTER_H
#define STANDARTSTABLESORTER_H

#include <algorithm>

template<typename DataType> class StandartStableSorter
{
	public:
		template<typename RandomAccessIterator>
		void operator () (RandomAccessIterator begin, RandomAccessIterator end)
		{
			std::stable_sort(begin, end);
		}

		template<typename RandomAccessIterator, typename Comparator>
		void operator () (RandomAccessIterator begin, RandomAccessIterator end, Comparator comparator)
		{
			std::stable_sort(begin, end, comparator);
		}
};

#endif // STANDARTSTABLESORTER_H
