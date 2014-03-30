#ifndef GRAPH_H
#define GRAPH_H

#include <utility>
#include <vector>
#include <memory>

#include "model/incidencelist.h"

namespace graph
{
	typedef std::size_t vertex_t;

	class Graph
	{
		public:
		private:
			std::vector< std::unique_ptr<IncidenceList> > adjLists;
	};
}

#endif // GENERALGRAPH_H
