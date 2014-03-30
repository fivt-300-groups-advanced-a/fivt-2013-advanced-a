#ifndef FUNCTIONALGRAPHFABRIC_H
#define FUNCTIONALGRAPHFABRIC_H

#include <vector>

#include "fabrics/abstractgraphfabric.h"
#include "lists/singlevertexlist.h"
#include "lists/vectorincidencelist.h"

namespace graph
{
	class FunctionalGraphFabric : public AbstractGraphFabric
	{
		public:
			FunctionalGraphFabric(std::vector<vertex_t> list, bool backEdges = false):
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
				assert(directPtr < to.size());
				return std::move(std::unique_ptr<IncidenceList>(new SingleVertexList(to[directPtr++])));
			}

			std::unique_ptr<IncidenceList> nextBackEdgeList() override
			{
				assert(back && backPtr < backList.size());
				return std::move(std::unique_ptr<IncidenceList>(new VectorIncidenceList(backList[backPtr++])));
			}

			virtual ~FunctionalGraphFabric() {}

		private:
			std::size_t directPtr, backPtr;
			std::vector<vertex_t> to;
			std::vector< std::vector<vertex_t> > backList;
			bool back;
	};
}

#endif // FUNCTIONALGRAPHFABRIC_H
