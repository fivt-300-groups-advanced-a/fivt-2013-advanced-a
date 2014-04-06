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
			VectorIncidenceList() {}
			VectorIncidenceList(std::vector<vertex_t> neighbours):
				adjList(neighbours)
			{
				if (!is_sorted(adjList.begin(), adjList.end()))
					performSort();
			}
			VectorIncidenceList(const std::initializer_list<vertex_t> &neighbours):
				adjList(neighbours)
			{
				if (!is_sorted(adjList.begin(), adjList.end()))
					performSort();
			}

			size_t size() const override
			{
				return adjList.size();
			}

			std::unique_ptr<IncidenceListIterator> getIterator() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>
								 (new StdIncidenceListIterator<std::vector<vertex_t>::const_iterator>(adjList.begin(), adjList.end())));
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
				sort(adjList.begin(), adjList.end());
			}
	};
}

#endif // VECTORINCIDENCELIST_H
