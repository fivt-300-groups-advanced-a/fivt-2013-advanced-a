#ifndef VECTORINCIDENCELIST_H
#define VECTORINCIDENCELIST_H

#include "lists/incidencelist.h"
#include "iterators/stdincidencelistiterator.h"

#include <algorithm>
#include <vector>

namespace graph
{
	class VectorIncidenceList : public IncidenceList
	{
		public:
			typedef StdIncidenceListIterator< std::vector<vertex_t>::const_iterator > iterator_type;

			VectorIncidenceList() {}

			template<typename ForwardIterator>
			VectorIncidenceList(ForwardIterator begin, ForwardIterator end):
				adjList(begin, end)
			{
				performSort();
			}
			VectorIncidenceList(std::initializer_list<vertex_t> neighbours):
				adjList(neighbours)
			{
				performSort();
			}

			size_t size() const override
			{
				return adjList.size();
			}

			iterator_pointer getIterator() const override
			{
				return std::move(iterator_pointer(new iterator_type(adjList.begin(), adjList.end())));
			}

			bool connected(vertex_t v) const override
			{
				return std::binary_search(adjList.begin(), adjList.end(), v);
			}

			virtual ~VectorIncidenceList() {}
		private:
			std::vector<vertex_t> adjList;

			void performSort()
			{
				if (!is_sorted(adjList.begin(), adjList.end()))
					sort(adjList.begin(), adjList.end());
			}
	};
}

#endif // VECTORINCIDENCELIST_H
