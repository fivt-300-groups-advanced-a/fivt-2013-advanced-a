#ifndef GRAPH_H
#define GRAPH_H

#include <utility>
#include <vector>
#include <memory>

#include "lists/incidencelist.h"

namespace graph
{
	typedef std::size_t vertex_t;

	class Graph
	{
		public:
			Graph(std::vector< std::unique_ptr<IncidenceList> > &edgesLists): adjLists(std::move(edgesLists)) {}
			Graph(std::vector< std::unique_ptr<IncidenceList> > &edgesLists,
				  std::vector< std::unique_ptr<IncidenceList> > &backEdgesLists):
				adjLists(std::move(edgesLists)), backLists(std::move(backEdgesLists)) {}

			std::unique_ptr<IncidenceList> getEdgesFrom(std::size_t id) const
			{
				return adjLists[id];
			}

			std::unique_ptr<IncidenceList> getEdgesTo(std::size_t id) const
			{
				return backLists[id];
			}

		private:
			std::vector< std::unique_ptr<IncidenceList> > adjLists, backLists;
	};
}

#endif // GENERALGRAPH_H
