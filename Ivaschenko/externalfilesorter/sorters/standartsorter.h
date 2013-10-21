#ifndef STANDARTSORTER_H
#define STANDARTSORTER_H

#include <algorithm>

template<typename DataType> class StandartSorter
{
	public:
		template<typename RandomAccessIterator>
		void operator () (RandomAccessIterator begin, RandomAccessIterator end)
		{
			std::sort(begin, end);
		}

		template<typename RandomAccessIterator, typename Comparator>
		void operator () (RandomAccessIterator begin, RandomAccessIterator end, Comparator comparator)
		{
			std::sort(begin, end, comparator);
		}
};

#endif // STANDARTSORTER_H
