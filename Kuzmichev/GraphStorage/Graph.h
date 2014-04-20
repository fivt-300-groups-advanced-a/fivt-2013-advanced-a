#ifndef GRAPH
#define GRAPH

#include <memory>
#include <vector>
#include <algorithm>
#include "LI.h"

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
	void dfs(vector <int> & order)
	{
		vector <bool> was(n, false);
		for (int i = 0; i < n; i++) if (!was[i])
			go(i, was, order);
	}
private:
	vectorLI inc;
	int n;
	void go(int v, vector <bool> & was, vector <int> & order)
	{
		//printf("v = %d\n", v);
		order.push_back(v);
		was[v] = true;
		IterPtr iterPtr = (*inc[v]).getIterator();
		while (!iterPtr->isEnd())
		{
			int u = iterPtr->getCurrentVertex();
			//printf("v = %d u = %d\n", v, u);
			if (!was[u])
				go(u, was, order);
			iterPtr->next();
		}
	}
	/*bool check()
	{
		for (int i = 0; i < inc.size(); i++)
		{
			if (typeof(inc[i]))
		}
	}*/
};

#endif