#ifndef ADJACENCYMATRIXITERATOR_H
#define ADJACENCYMATRIXITERATOR_H

#include "iterators/incidencelistiterator.h"
#include "lists/incidencelist.h"

namespace graph
{
	class AdjacencyMatrixIterator : public IncidenceListIterator
	{
		public:
			AdjacencyMatrixIterator(const IncidenceList *list, vertex_t begin, vertex_t end):
				matrixRow(list), current(begin), last(end)
			{
				normalize();
			}

			vertex_t operator * () const override
			{
				assert(current < last);
				return current;
			}

			bool moveForward() override
			{
				++current;
				normalize();
				return valid();
			}

			bool valid() const override
			{
				return current < last;
			}


		private:
			const IncidenceList *matrixRow;
			vertex_t current, last;

			void normalize()
			{
				while (current < last && !matrixRow->connected(current))
					++current;
			}
	};
}

#endif // ADJACENCYMATRIXITERATOR_H
