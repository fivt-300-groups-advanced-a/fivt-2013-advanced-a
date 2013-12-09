#include "biheap.h"

template <class Iterator, class Compare>

void heapsort(Iterator begin, Iterator end, Compare cmp)
{
	BiHeap<typename std::remove_reference<decltype(*begin)>::type, Compare>heap(cmp);
	for (Iterator it = begin; it != end; ++it)
		heap.insert(*it);
	for (Iterator it = begin; it != end; ++it)
	{
		*it = heap.top();
		heap.pop();
	}
}