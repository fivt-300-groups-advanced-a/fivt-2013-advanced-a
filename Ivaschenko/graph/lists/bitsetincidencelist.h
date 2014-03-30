#ifndef BITSETINCIDENCELIST_H
#define BITSETINCIDENCELIST_H

#include "lists/incidencelist.h"
#include "iterators/adjacencymatrixiterator.h"

#include <utility>
#include <bitset>

namespace graph
{
	template<std::size_t graphSize> class BitsetIncidenceList : public IncidenceList
	{
		public:
			BitsetIncidenceList(): bitCount(0)
			{
				isNeighbour.reset();
			}

			BitsetIncidenceList(std::bitset<graphSize> &bitSet): isNeighbour(bitSet), bitCount(isNeighbour.count()) {}
			BitsetIncidenceList(const std::initializer_list<vertex_t> &list)
			{
				isNeighbour.reset();
				for (vertex_t v : list) isNeighbour.set(v);
				bitCount = isNeighbour.count();
			}

			std::size_t size() const override
			{
				return bitCount;
			}

			std::unique_ptr<IncidenceListIterator> getIterator() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>
								 (new AdjacencyMatrixIterator(this, 0, graphSize - 1)));
			}

			bool connected(vertex_t v) const override
			{
				return isNeighbour[v];
			}

			virtual ~BitsetIncidenceList() {}

		private:
			std::bitset<graphSize> isNeighbour;
			std::size_t bitCount;
	};
}

#endif // BITSETINCIDENCELIST_H
