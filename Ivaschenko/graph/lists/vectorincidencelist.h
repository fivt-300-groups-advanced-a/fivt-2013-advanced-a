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
			VectorIncidenceList(): sorted(true) {}
			VectorIncidenceList(std::vector<vertex_t> neighbours, bool sortNeeded):
				adjList(neighbours), sorted(sortNeeded)
			{
				if (sortNeeded) performSort();
			}
			VectorIncidenceList(const std::initializer_list<vertex_t> &neighbours, bool sortNeeded):
				adjList(neighbours), sorted(sortNeeded)
			{
				if (sortNeeded) performSort();
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
				if (sorted)
					return std::binary_search(adjList.begin(), adjList.end(), v);
				return std::find(adjList.begin(), adjList.end(), v) != adjList.end();
			}

			virtual ~VectorIncidenceList() {}
		private:
			std::vector<vertex_t> adjList;
			bool sorted;

			void performSort()
			{
				sort(adjList.begin(), adjList.end());
			}
	};
}

#endif // VECTORINCIDENCELIST_H
