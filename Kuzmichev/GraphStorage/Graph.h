#ifndef GRAPH
#define GRAPH

#include <memory>
#include <vector>
#include <algorithm>
#include "LI.h"
#include "Factory.h"

using namespace std;

class Graph
{
public:
	Graph(vectorLI & inc_)
	{
		inc = move(inc_);
		n = inc.size();
	}
	AbstractLI & getIncidents(uint v)
	{
		return *inc[v];
	}
	
	const uint getN()
	{
		return n;
	}

private:
	vectorLI inc;
	uint n;
};

typedef unique_ptr <Graph> GraphPtr;

#endif