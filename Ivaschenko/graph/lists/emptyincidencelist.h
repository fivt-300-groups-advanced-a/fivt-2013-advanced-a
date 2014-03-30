#ifndef EMPTYINCIDENCELIST_H
#define EMPTYINCIDENCELIST_H

#include <memory>

#include "lists/incidencelist.h"
#include "iterators/adjacencymatrixiterator.h"

namespace graph
{
	class EmptyIncidenceList : public IncidenceList
	{
		public:
			EmptyIncidenceList() {}

			std::size_t size() const override
			{
				return 0;
			}

			std::unique_ptr<IncidenceListIterator> getIterator() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>(new AdjacencyMatrixIterator(this, 1, 0)));
			}

			bool connected(vertex_t) const override
			{
				return false;
			}

			virtual ~EmptyIncidenceList() {}
	};
}

#endif // EMPTYINCIDENCELIST_H
