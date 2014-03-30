#ifndef BITSETGRAPHFABRIC_H
#define BITSETGRAPHFABRIC_H

#include "fabrics/abstractgraphfabric.h"
#include "lists/bitsetincidencelist.h"

namespace graph
{
	class BitsetGraphFabric : public AbstractGraphFabric
	{
		public:
			BitsetGraphFabric(std::vector< std::vector<vertex_t> > list, bool backEdges = false):
				directPtr(0), backPtr(0), adjList(list), back(backEdges)
			{
				if (backEdges)
				{
					backList.resize(list.size());
					for (size_t v = 0; v < list.size(); ++v)
						for (int to : list[v])
							backList[to].push_back(v);
				}
			}

			std::unique_ptr<IncidenceList> nextEdgeList() override
			{
				assert(directPtr < adjList.size());
				return std::move(std::unique_ptr<IncidenceList>(new BitsetIncidenceList(adjList.size(), adjList[directPtr++])));
			}

			std::unique_ptr<IncidenceList> nextBackEdgeList() override
			{
				assert(back && backPtr < backList.size());
				return std::move(std::unique_ptr<IncidenceList>(new BitsetIncidenceList(adjList.size(), backList[backPtr++])));
			}

			virtual ~BitsetGraphFabric() {}

		private:
			std::size_t directPtr, backPtr;
			std::vector< std::vector<vertex_t> > adjList, backList;
			bool back;
	};
}

#endif // BITSELGRAPHFABRIC_H
