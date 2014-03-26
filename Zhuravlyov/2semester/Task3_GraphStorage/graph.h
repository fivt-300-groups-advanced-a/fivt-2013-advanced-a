#ifndef GRAPH
#define GRAPH

#include "ListOfInc.h"

class Graph
{
public:
	// i have no idea what to do
	// how can user change storage way?
private:
	std::vector< std::unique_ptr<ListOfIncedents> > edges_;
	std::vector< std::unique_ptr<ListOfIncedents> > back_edges_;
};

#endif