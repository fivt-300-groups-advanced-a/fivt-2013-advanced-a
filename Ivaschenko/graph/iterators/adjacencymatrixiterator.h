#ifndef ADJACENCYMATRIXITERATOR_H
#define ADJACENCYMATRIXITERATOR_H

#include "iterators/incidencelistiterator.h"
#include "lists/incidencelist.h"

namespace graph
{
	class AdjacencyMatrixIterator : public IncidenceListIterator
	{
		public:
			AdjacencyMatrixIterator(const IncidenceList *list, vertex_t start, vertex_t finish):
				matrixRow(list), current(start), last(finish)
			{
				normalize();
			}

			vertex_t operator * () const override
			{
				return current;
			}

			std::unique_ptr<IncidenceListIterator> next() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>(
								 new AdjacencyMatrixIterator(matrixRow, current + 1, last)));
			}

			bool moveForward() override
			{
				++current;
				normalize();
				return valid();
			}

			bool valid() const override
			{
				return current <= last;
			}


		private:
			const IncidenceList *matrixRow;
			vertex_t current, last;

			void normalize()
			{
				while (current <= last && !matrixRow->connected(current))
					++current;
			}
	};
}

#endif // ADJACENCYMATRIXITERATOR_H
