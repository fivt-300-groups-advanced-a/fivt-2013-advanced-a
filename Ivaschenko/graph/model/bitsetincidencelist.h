#ifndef BITSETINCIDENCELIST_H
#define BITSETINCIDENCELIST_H

#include "model/incidencelist.h"

#include <utility>
#include <bitset>

namespace graph
{

	template<int graphSize> class BitsetIncidenceListIterator : public IncidenceListIterator
	{
		public:
			BitsetIncidenceListIterator(std::size_t pos, const std::bitset<graphSize> &neighbour):
				position(pos), isNeighbour(neighbour)
			{
				normalisePosition();
			}

			vertex_t operator * () const override
			{
				return vertex_t(position);
			}

			std::unique_ptr<IncidenceListIterator> next() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>
								 (new BitsetIncidenceListIterator<graphSize>(position + 1, isNeighbour)));
			}

			void moveForward() override
			{
				assert(position < graphSize);
				++position;
				normalisePosition();
			}

			bool operator == (const std::unique_ptr<IncidenceListIterator> &it) const override
			{
				BitsetIncidenceListIterator<graphSize>* bit =
						dynamic_cast<BitsetIncidenceListIterator<graphSize>*>(it.get()); // ohhh...
				assert(bit != nullptr && &isNeighbour == &bit->isNeighbour);
				return position == bit->position;
			}

			bool operator != (const std::unique_ptr<IncidenceListIterator> &it) const override
			{
				BitsetIncidenceListIterator<graphSize>* bit =
						dynamic_cast<BitsetIncidenceListIterator<graphSize>*>(it.get()); // ohhh...
				assert(bit != nullptr && &isNeighbour == &bit->isNeighbour);
				return position != bit->position;
			}

			virtual ~BitsetIncidenceListIterator() {}

		private:
			std::size_t position;
			const std::bitset<graphSize> &isNeighbour;

			void normalisePosition()
			{
				while (position < graphSize && !isNeighbour[position])
					++position;
			}
	};

	template<int graphSize> class BitsetIncidenceList : public IncidenceList
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

			std::unique_ptr<IncidenceListIterator> begin() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>
								 (new BitsetIncidenceListIterator<graphSize>(0, isNeighbour)));
			}
			std::unique_ptr<IncidenceListIterator> end() const override
			{
				return std::move(std::unique_ptr<IncidenceListIterator>
								 (new BitsetIncidenceListIterator<graphSize>(graphSize, isNeighbour)));
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
