#ifndef GRAPH_H
#define GRAPH_H

#include <utility>
#include <vector>
#include <memory>

#include "lists/incidencelist.h"
#include "fabrics/abstractgraphfabric.h"

namespace graph
{
	typedef std::size_t vertex_t;

	class Graph
	{
		public:
			explicit Graph(std::vector< std::unique_ptr<IncidenceList> > &edgesLists): adjLists(std::move(edgesLists)) {}

			Graph(std::vector< std::unique_ptr<IncidenceList> > &edgesLists,
				  std::vector< std::unique_ptr<IncidenceList> > &backEdgesLists):
				adjLists(std::move(edgesLists)), backLists(std::move(backEdgesLists)) {}

			Graph(const std::unique_ptr<AbstractGraphFabric> &fabric, std::size_t graphSize, bool reverseEdges)
			{
				adjLists.resize(graphSize);
				if (reverseEdges) backLists.resize(graphSize);
				for (std::size_t i = 0; i < graphSize; ++i)
				{
					adjLists[i] = std::move(fabric->nextEdgeList());
					if (reverseEdges)
						backLists[i] = std::move(fabric->nextBackEdgeList());
				}
			}

			std::size_t size() const
			{
				return adjLists.size();
			}

			const std::unique_ptr<IncidenceList>& getEdgesFrom(std::size_t id) const
			{
				return std::move(adjLists[id]);
			}

			const std::unique_ptr<IncidenceList>& getEdgesTo(std::size_t id) const
			{
				return std::move(backLists[id]);
			}

		private:
			std::vector< std::unique_ptr<IncidenceList> > adjLists, backLists;
	};
}

#endif // GENERALGRAPH_H
