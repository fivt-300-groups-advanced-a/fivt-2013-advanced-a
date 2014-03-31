#ifndef ABSTRACTGRAPHFABRIC_H
#define ABSTRACTGRAPHFABRIC_H

#include <memory>

#include <lists/incidencelist.h>

namespace graph
{
	class AbstractGraphFabric
	{
		public:
			virtual std::unique_ptr<IncidenceList> nextEdgeList() = 0;
			virtual std::unique_ptr<IncidenceList> nextBackEdgeList() = 0;
			virtual ~AbstractGraphFabric() {}
	};
}

#endif // ABSTRACTGRAPHFABRIC_H
