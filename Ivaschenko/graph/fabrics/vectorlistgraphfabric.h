#ifndef VECTORLISTGRAPHFABRIC_H
#define VECTORLISTGRAPHFABRIC_H

#include <vector>

#include "lists/vectorincidencelist.h"
#include "fabrics/abstractgraphfabric.h"

namespace graph
{
	class VectorListGraphFabric : public AbstractGraphFabric
	{
		public:
			VectorListGraphFabric(std::vector< std::vector<vertex_t> > list,
								  bool backEdges = false, bool sortNeeded = false):
				directPtr(0), backPtr(0), adjList(list), sort(sortNeeded), back(backEdges)
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
				return std::move(std::unique_ptr<IncidenceList>(new VectorIncidenceList(adjList[directPtr++], sort)));
			}

			std::unique_ptr<IncidenceList> nextBackEdgeList() override
			{
				assert(back && backPtr < backList.size());
				return std::move(std::unique_ptr<IncidenceList>(new VectorIncidenceList(backList[backPtr++], sort)));
			}

			virtual ~VectorListGraphFabric() {}

		private:
			std::size_t directPtr, backPtr;
			std::vector< std::vector<vertex_t> > adjList, backList;
			bool sort, back;

	};
}

#endif // VECTORLISTGRAPHFABRIC_H
