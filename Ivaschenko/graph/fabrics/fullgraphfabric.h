#ifndef FULLGRAPHFABRIC_H
#define FULLGRAPHFABRIC_H

#include "fabrics/abstractgraphfabric.h"
#include "lists/consecutiveincidencelist.h"

namespace graph
{
	class FullGraphFabric : public AbstractGraphFabric
	{
		public:
			FullGraphFabric(std::size_t n): graphSize(n), directPtr(0), backPtr(0) {}

			std::unique_ptr<IncidenceList> nextEdgeList() override
			{
				assert(directPtr < graphSize);
				++directPtr;
				return std::move(std::unique_ptr<IncidenceList>(
									 new ConsecutiveIncidenceList(0, graphSize - 1)));
			}

			std::unique_ptr<IncidenceList> nextBackEdgeList() override
			{
				assert(backPtr < graphSize);
				++backPtr;
				return std::move(std::unique_ptr<IncidenceList>(
									 new ConsecutiveIncidenceList(0, graphSize - 1)));
			}

			virtual ~FullGraphFabric() {}

		private:
			std::size_t graphSize;
			std::size_t directPtr, backPtr;

	};
}

#endif // FULLGRAPHFABRIC_H
