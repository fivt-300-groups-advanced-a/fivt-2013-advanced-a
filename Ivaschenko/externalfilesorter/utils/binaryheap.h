#ifndef BINARYHEAP_H
#define BINARYHEAP_H

#include <utility>

/*
 * Utility class containing realisation of binary heap
 * Can store std::pair<DataType, int>
 * Unstable. Does not store data.
 * Optimised for ExternalSorter use only
 */
template <typename DataType> class BinaryHeap
{
	public:
		std::pair<DataType, int> *data;
		std::size_t size;

		/*
		 * Constructs heap from array (heap) of size (tempFiles)
		 * Has linear complexity (at most 2 * tempFiles calls of comparator())
		 */
		template<typename Comparator> void construct(std::size_t size, Comparator &comparator)
		{
			size = 0;
			for (std::size_t i = 0; i < size; i++)
				push(comparator);
		}

		/*
		 * Pops min element from heap
		 * Does not more than 2 * log(heapSize) calls of comparator()
		 */
		template<typename Comparator> void pop(Comparator &comparator)
		{
			assert(size > 0);
			swap(data[0], data[--size]);
			std::size_t curpos = 0;
			while (2 * curpos + 1 < size)
			{
				std::size_t child = 2 * curpos + 1;
				if (child + 1 < size && comparator(data[child + 1].first, data[child].first))
					++child;
				if (comparator(data[child].first, data[curpos].first))
					data[curpos].swap(data[child]), curpos = child;
				else break;
			}
		}

		/*
		 * Inserts element to heap assuming that it is in position (heapSize) from begining of (heap)
		 * Does not more than log(heapSize) calls of comparator()
		 */
		template<typename Comparator> void push(Comparator &comparator)
		{
			size_t curpos = size++;
			while (curpos != 0)
			{
				if (comparator(data[curpos].first, data[(curpos - 1) >> 1].first))
					data[curpos].swap(data[(curpos - 1) >> 1]);
				curpos = (curpos - 1) >> 1;
			}
		}
};

#endif // BINARYHEAP_H
