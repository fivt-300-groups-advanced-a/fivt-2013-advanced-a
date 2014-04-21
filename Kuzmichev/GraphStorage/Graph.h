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
	const void dfs(vector <uint> & order)
	{
		vector <bool> was(n, false);
		for (uint i = 0; i < n; i++) if (!was[i])
			go(i, was, order);
	}

	const int getN()
	{
		return n;
	}

	

	Graph condensate(vector <uint> & injection)
	{
		vector <uint> order;
		dfs(order);
		assert(order.size() == n);

		vector <StandardFactory> factG(n, StandardFactory(n));
		/*for (uint v = 0; v < n; v++)
			factG[v] = StandardFactory(n);*/
		for (uint v = 0; v < n; v++)
		{
			IterPtr iterPtr = (*inc[v]).getIterator();
			while (!iterPtr->isEnd())
			{
				uint u = iterPtr->getCurrentVertex();
				factG[u].addEdge(v);
				iterPtr->next();
			}
		}
		vectorLI newGVector;
		for (uint j = 0; j < n; j++)
			newGVector.push_back(move(factG[j].getLI()));
			//newGVector[j] = move(factG[j].getLI());
		Graph newG(newGVector);

		vector <bool> was2(n, false);
		uint cnt = 0;
		injection.resize(n);
		for (int j = n - 1; j >= 0; j--) if (!was2[order[j]])
		{
			newG.dfs2(order[j], was2, injection, cnt);
			cnt++;
		}
		vector <StandardFactory> fact(cnt, StandardFactory(cnt));
		/*for (uint v = 0; v < cnt; v++)
			fact[v] = StandardFactory(cnt);*/
		for (uint v = 0; v < n; v++)
		{
			IterPtr iterPtr = (*inc[v]).getIterator();
			while (!iterPtr->isEnd())
			{
				uint u = iterPtr->getCurrentVertex();
				if (injection[v] != injection[u])
					fact[injection[v]].addEdge(injection[u]);
				iterPtr->next();
			}
		}
		vectorLI vect;
		for (uint j = 0; j < cnt; j++)
			vect.push_back(move(fact[j].getLI()));
		return Graph(vect);
	}

	bool checkConnection()
	{
		vector <bool> was(n, false);
		vector <uint> order;
		go(0, was, order);
		return order.size() == n;
	}

	/*int getOptimalAnswer()
	{
		
		vector <bool> source(n, true);
		for (uint v = 0; v < n; v++)
		{
			IterPtr iter = (*inc[v]).getIterator();
			while (!iter->isEnd())
			{
				int u = iter->getCurrentVertex();
				source[u] = false;
				//	sourcesCnt--;
				iter->next();
			}
		}
		int sourceCnt = 0;
		for (uint v = 0; v < n; v++)
			if (source[v])
				sourceCnt++;
		int sinkCnt = 0;
		for (uint v = 0; v < n; v++)
			if (inc[v]->getIterator()->isEnd())
				sinkCnt++;
		return max(sinkCnt, sourceCnt);
	}*/

	//void connectivityProblem(vector <pair <uint, uint> > & edgesToAdd)
	void connectivityProblem(vector <pair <uint, uint> > & edgesToAdd, int & optimalAnswer)
	{
		vector <uint> injection;
		Graph C = condensate(injection);
		uint cn = C.getN();
		vector <uint> r(cn);
		for (uint j = 0; j < n; j++)
			r[injection[j]] = j;
		vector <bool> mark(cn, false);
		//int sourcesCnt = n;
		vector <bool> source(cn, true);
		vector <bool> sink(cn);
		for (uint v = 0; v < cn; v++)
		{
			IterPtr iter = (*C.inc[v]).getIterator();
			while (!iter->isEnd())
			{
				int u = iter->getCurrentVertex();
				source[u] = false;
			//	sourcesCnt--;
				iter->next();
			}
		}
		int sourcesCnt = 0;
		for (uint v = 0; v < cn; v++)
			if (source[v])
				sourcesCnt++;
		int sinksCnt = 0;
		for (uint v = 0; v < cn; v++)
		{
			sink[v] = C.inc[v]->getIterator()->isEnd();
			if (sink[v])
				sinksCnt++;
		}
		optimalAnswer = max(sourcesCnt, sinksCnt);
		vector <uint> sourcesList, sinksList;
		for (uint v = 0; v < cn; v++)
			if (!mark[v] && source[v])
			{
				int w = C.find_sink(v, mark);
				if (w != -1)
				{
					sourcesList.push_back(v);
					sinksList.push_back(w);
				}
			}
		
		uint source0, sink0;
		if (!sourcesList.empty())
		{
			source0 = sourcesList[0];
			sink0 = sinksList[0];
		}
		
		for (uint i = 0; i < sourcesList.size(); i++)
		{
			int S = sinksList[(i + 1) % sinksList.size()];
			edgesToAdd.push_back(make_pair(r[S], r[sourcesList[i]]));
			source[sourcesList[i]] = false;
			sink[S] = false;
		}
		
		sourcesList.clear();
		sinksList.clear();
		for (uint v = 0; v < cn; v++)
		{
			if (source[v])
				sourcesList.push_back(v);
			if (sink[v])
				sinksList.push_back(v);
		}
		//sourcesCnt -= sources.size();
		//sinksCnt -= sources.size();
		for (uint i = 0; i < min(sourcesList.size(), sinksList.size()); i++)
		{
			edgesToAdd.push_back(make_pair(r[sinksList[i]], r[sourcesList[i]]));
		}
		int chosenSink = (sinksList.empty() ? r[sink0] : r[sinksList[0]]);
		int chosenSource = (sourcesList.empty() ? r[source0] : r[sourcesList[0]]);
		if (sourcesList.size() > sinksList.size())
		{
			for (uint i = sinksList.size(); i < sourcesList.size(); i++)
				edgesToAdd.push_back(make_pair(chosenSink, r[sourcesList[i]]));
		}
		else if (sourcesList.size() < sinksList.size())
		{
			for (uint i = sourcesList.size(); i < sinksList.size(); i++)
				edgesToAdd.push_back(make_pair(r[sinksList[i]], chosenSource));
		}
	}
	void go(const uint v, vector <bool> & was, vector <uint> & order) const
	{
		//printf("v = %d\n", v);

		was[v] = true;
		IterPtr iterPtr = (*inc[v]).getIterator();
		while (!iterPtr->isEnd())
		{
			uint u = iterPtr->getCurrentVertex();
			//printf("v = %d u = %d\n", v, u);
			if (!was[u])
				go(u, was, order);
			iterPtr->next();
		}
		order.push_back(v);
	}
private:
	vectorLI inc;
	uint n;
	int find_sink(const uint v, vector <bool> & mark) const
	{
		mark[v] = true;
		if (inc[v]->getIterator()->isEnd())
			return v;
		IterPtr iterPtr = (*inc[v]).getIterator();
		while (!iterPtr->isEnd())
		{
			int u = iterPtr->getCurrentVertex();
			if (!mark[u])
			{
				int tmp = find_sink(u, mark);
				if (tmp != -1)
					return tmp;
			}
			iterPtr->next();
		}
		return -1;
	}
	
	void dfs2(const uint v, vector <bool> & was, vector <uint> & injection, const int number) const
	{
		was[v] = true;
		injection[v] = number;
		//IterPtr iterPtr = move((*inc[v]).getIterator());
		IterPtr iterPtr = (*inc[v]).getIterator();
		while (!iterPtr->isEnd())
		{
			uint u = iterPtr->getCurrentVertex();
			//printf("v = %d u = %d\n", v, u);
			if (!was[u])
				dfs2(u, was, injection, number);
			iterPtr->next();
		}
	}
};

#endif