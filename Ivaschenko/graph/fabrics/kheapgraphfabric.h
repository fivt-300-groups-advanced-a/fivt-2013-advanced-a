#ifndef KHEAPGRAPHFABRIC_H
#define KHEAPGRAPHFABRIC_H

#include "fabrics/abstractgraphfabric.h"

#include "lists/consecutiveincidencelist.h"
#include "lists/emptyincidencelist.h"
#include "lists/singlevertexlist.h"

namespace graph
{
	class KHeapGraphFabric : public AbstractGraphFabric
	{
		public:
			KHeapGraphFabric(std::size_t graphSize, std::size_t childrenCount):
				n(graphSize), k(childrenCount), directPtr(0), backPtr(0) {}

			std::unique_ptr<IncidenceList> nextEdgeList() override
			{
				assert(directPtr < n);
				std::unique_ptr<IncidenceList> ret;
				std::size_t last = std::min(directPtr * k + k, n - 1);
				if (directPtr * k + 1 >= n)
					ret = std::move(std::unique_ptr<IncidenceList>(new EmptyIncidenceList()));
				else
					ret = std::move(std::unique_ptr<IncidenceList>(
									 new ConsecutiveIncidenceList(directPtr * k + 1, last)));
				++directPtr;
				return std::move(ret);
			}

			std::unique_ptr<IncidenceList> nextBackEdgeList() override
			{
				assert(backPtr < n);
				std::unique_ptr<IncidenceList> ret;
				if (backPtr == 0) ret = std::move(std::unique_ptr<IncidenceList>(new EmptyIncidenceList()));
				else ret = std::move(std::unique_ptr<IncidenceList>(new SingleVertexList((backPtr - 1) / k)));
				++backPtr;
				return std::move(ret);
			}

			virtual ~KHeapGraphFabric() {}

		private:
			std::size_t n, k;
			std::size_t directPtr, backPtr;
	};
}

#endif // KHEAPGRAPHFABRIC_H
