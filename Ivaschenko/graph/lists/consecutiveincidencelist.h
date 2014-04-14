#ifndef CONSECUTIVEINCIDENCELIST_H
#define CONSECUTIVEINCIDENCELIST_H

#include "lists/incidencelist.h"
#include "iterators/adjacencymatrixiterator.h"

#include <memory>

namespace graph
{
	class ConsecutiveIncidenceList : public IncidenceList
	{
		public:
			ConsecutiveIncidenceList(std::size_t begin, std::size_t end):
				first(begin), last(end)
			{
				assert(first <= last);
			}

			std::size_t size() const override
			{
				return last - first + 1;
			}

			std::unique_ptr<IncidenceListIterator> getIterator() const override
			{
				// TODO: open interval
				return std::move(std::unique_ptr<IncidenceListIterator>
								 (new AdjacencyMatrixIterator(this, first, last)));
			}

			bool connected(vertex_t v) const override
			{
				return first <= v && v <= last;
			}

			virtual ~ConsecutiveIncidenceList() {}

		private:
			std::size_t first, last;

	};
}

#endif // CONSECUTIVEINCIDENCELIST_H
