#ifndef GRAPH_H
#define GRAPH_H

#include <utility>
#include <vector>
#include <memory>

#include "lists/incidencelist.h"
#include "fabrics/defaultlistbuilder.h"

namespace graph
{
	typedef std::size_t vertex_t;

	class Graph
	{
		public:
			Graph(const Graph &g) = delete;
			Graph operator = (const Graph &g) = delete;

			explicit Graph(std::vector< std::unique_ptr<IncidenceList> > &edgesLists): adjLists(std::move(edgesLists)) {}

			Graph(std::vector< std::unique_ptr<IncidenceList> > &edgesLists,
				  std::vector< std::unique_ptr<IncidenceList> > &backEdgesLists):
				adjLists(std::move(edgesLists)), backLists(std::move(backEdgesLists)) {}

			std::size_t size() const
			{
				return adjLists.size();
			}

			const IncidenceList* getEdgesFrom(std::size_t id) const
			{
				assert(id < adjLists.size());
				return adjLists[id].get();
			}

			const IncidenceList* getEdgesTo(std::size_t id) const
			{
				assert(id < backLists.size());
				return backLists[id].get();
			}

		private:
			std::vector< std::unique_ptr<IncidenceList> > adjLists, backLists;
	};
}

#endif // GENERALGRAPH_H
