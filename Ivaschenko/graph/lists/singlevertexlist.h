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
			SingleVertexList(vertex_t v): to(v) {}

			std::size_t size() const override
			{
				return 1;
			}

			std::unique_ptr<IncidenceListIterator> getIterator() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>(new AdjacencyMatrixIterator(this, to, to)));
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
