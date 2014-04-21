#ifndef SINGLEVERTEXLIST_H
#define SINGLEVERTEXLIST_H

#include "lists/incidencelist.h"
#include "iterators/adjacencymatrixiterator.h"

#include <memory>

namespace graph
{
	class SingleVertexList : public IncidenceList
	{
		public:
			typedef AdjacencyMatrixIterator iterator_type;

			explicit SingleVertexList(vertex_t v): to(v) {}

			std::size_t size() const override
			{
				return 1;
			}

			iterator_pointer getIterator() const override
			{
				return std::move(iterator_pointer(new iterator_type(this, to, to + 1)));
			}

			bool connected(vertex_t v) const override
			{
				return v == to;
			}

			virtual ~SingleVertexList() {}

		private:
			vertex_t to;
	};
}

#endif // SINGLEVERTEXLIST_H
