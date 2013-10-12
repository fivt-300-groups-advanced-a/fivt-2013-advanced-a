#ifndef STABLEBINARYHEAP_H
#define STABLEBINARYHEAP_H

#include <utility>

/*
 * Stable analog of BinaryHeap
 * Can store std::pair<DataType, int>
 * Stable. Does not store data.
 * Optimised for ExternalSorter use only
 */
template <typename DataType> class StableBinaryHeap
{
	public:
		std::pair<DataType, int> *data;
		std::size_t size;

		/**
		 * Constructs heap from array (heap) of size (tempFiles)
		 * Has linear complexity (at most 4 * tempFiles calls of comparator())
		 */
		template<typename Comparator> void construct(std::size_t newSize, Comparator &comparator)
		{
			size = 0;
			for (std::size_t i = 0; i < newSize; i++)
				push(comparator);
		}

		/**
		 * Pops min element from heap
		 * Does not more than 4 * log(heapSize) calls of comparator()
		 */
		template<typename Comparator> void pop(Comparator &comparator)
		{
			assert(size > 0);
			swap(data[0], data[--size]);
			std::size_t curpos = 0;
			while (2 * curpos + 1 < size)
			{
				std::size_t child = 2 * curpos + 1;
				if (child + 1 < size && comparePair(data[child + 1], data[child], comparator))
					++child;
				if (comparePair(data[child], data[curpos], comparator))
					data[curpos].swap(data[child]), curpos = child;
				else break;
			}
		}

		/**
		 * Inserts element to heap assuming that it is in position (heapSize) from begining of (heap)
		 * Does not more than 2 * log(heapSize) calls of comparator()
		 */
		template<typename Comparator> void push(Comparator &comparator)
		{
			size_t curpos = size++;
			while (curpos != 0)
			{
				if (comparePair(data[curpos], data[(curpos - 1) >> 1], comparator))
					data[curpos].swap(data[(curpos - 1) >> 1]);
				curpos = (curpos - 1) >> 1;
			}
		}

	private:
		/**
		 * Lexicographic operator <
		 */
		template<typename Comparator>
		bool comparePair(const std::pair<DataType, int> &a, const std::pair<DataType, int> &b, Comparator &cmp)
		{
			return cmp(a.first, b.first) || (!cmp(b.first, a.first) && a.second < b.second);
		}
};

#endif // STABLEBINARYHEAP_H
