#ifndef BITSETINCIDENCELIST_H
#define BITSETINCIDENCELIST_H

#include "lists/incidencelist.h"
#include "iterators/adjacencymatrixiterator.h"

#include <utility>
#include <vector>

namespace graph
{
	class BitsetIncidenceList : public IncidenceList
	{
		public:
			explicit BitsetIncidenceList(std::size_t graphSize): bitCount(0)
			{
				isNeighbour.assign(graphSize, false);
			}

			explicit BitsetIncidenceList(const std::vector<bool> &bitSet): isNeighbour(bitSet)
			{
				bitCount = 0;
				for (std::size_t v = 0; v < isNeighbour.size(); ++v)
					if (isNeighbour[v]) ++bitCount;
			}

			template<typename ForwardIterator>
			BitsetIncidenceList(std::size_t graphSize, ForwardIterator begin, ForwardIterator end)
			{
				isNeighbour.assign(graphSize, false);
				for (; begin != end; ++begin)
					isNeighbour[*begin] = true;
				bitCount = 0;
				for (std::size_t v = 0; v < graphSize; ++v)
					if (isNeighbour[v]) ++bitCount;
			}

			std::size_t size() const override
			{
				return bitCount;
			}

			std::unique_ptr<IncidenceListIterator> getIterator() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>
								 (new AdjacencyMatrixIterator(this, 0, isNeighbour.size() - 1)));
			}

			bool connected(vertex_t v) const override
			{
				return isNeighbour[v];
			}

			virtual ~BitsetIncidenceList() {}

		private:
			std::vector<bool> isNeighbour;
			std::size_t bitCount;
	};
}

#endif // BITSETINCIDENCELIST_H
