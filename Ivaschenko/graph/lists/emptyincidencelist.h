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
			typedef AdjacencyMatrixIterator iterator_type;

			EmptyIncidenceList() {}

			std::size_t size() const override
			{
				return 0;
			}

			iterator_pointer getIterator() const override
			{
				return std::move(iterator_pointer(new iterator_type(this, 0, 0)));
			}

			bool connected(vertex_t) const override
			{
				return false;
			}

			virtual ~EmptyIncidenceList() {}
	};
}

#endif // EMPTYINCIDENCELIST_H
