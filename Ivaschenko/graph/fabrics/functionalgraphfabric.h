#ifndef FUNCTIONALGRAPHFABRIC_H
#define FUNCTIONALGRAPHFABRIC_H

#include <vector>

#include "fabrics/abstractgraphfabric.h"
#include "lists/singlevertexlist.h"

namespace graph
{
	class FunctionalGraphFabric : public AbstractGraphFabric
	{
		public:
			FunctionalGraphFabric(std::vector<vertex_t> to, bool backEdges = false):
				directPtr(0), backPtr(0), to(list), back(backEdges)
			{
				if (backEdges)
				{
					backList.resize(to.size());
					for (size_t v = 0; v < to.size(); ++v)
						backList[to[v]].push_back(v);
				}
			}

			std::unique_ptr<IncidenceList> nextEdgeList() override
			{
				assert(directPtr < adjList.size());
				return std::move(std::unique_ptr<IncidenceList>(new SingleVertexList(adjList.size(), adjList[directPtr++])));
			}

			std::unique_ptr<IncidenceList> nextBackEdgeList() override
			{
				assert(back && backPtr < backList.size());
				return std::move(std::unique_ptr<IncidenceList>(new BitsetIncidenceList(adjList.size(), backList[backPtr++])));
			}

			virtual ~BitsetGraphFabric() {}

		private:
			std::size_t directPtr, backPtr;
			std::vector<vertex_t> to;
			std::vector< std::vector<vertex_t> > backList;
			bool back;
	};
}

#endif // FUNCTIONALGRAPHFABRIC_H
