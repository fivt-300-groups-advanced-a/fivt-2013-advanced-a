#include "Graph.h"
#include <memory>
#include <vector>
#include <stack>

using namespace std;

class GraphAlgo
{
public:
	GraphAlgo(Graph * _G)
	{
		G = _G;
		n = G->getN();
	}

	const static int WHITE = 0;
	const static int GREY = 1;
	const static int BLACK = 2;

	const bool checkConnection()
	{
		vector <bool> was(n, false);
		vector <uint> order;
		dfs(0, was, order);
		return order.size() == n;
	}

	const void dfsFromAllVertex(vector <uint> & order)
	{
		vector <bool> was(n, false);
		for (uint i = 0; i < n; i++) if (!was[i])
			dfs(i, was, order);
	}

	void buildInjection(vector <uint> & injection, uint & compCnt)
	{
		injection.resize(n);
		vector <int> status(n, WHITE);
		vector <int> inTime(n);
		vector <int> up(n);
		stack <int> st;
		uint curTime = 0;
		for (uint v = 0; v < n; v++) if (status[v] == WHITE)
			tarjanAlgo(v, status, st, up, inTime, curTime, compCnt, injection);
	}

	Graph condensate(vector <uint> & injection)
	{
		uint cnt = 0;
		buildInjection(injection, cnt);

		vector <StandardFactory> fact(cnt, StandardFactory(cnt));
		/*for (uint v = 0; v < cnt; v++)
		fact[v] = StandardFactory(cnt);*/
		for (uint v = 0; v < n; v++)
		{
			IterPtr iterPtr = (getInc(v)).getIterator();
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

	int getEdgesToConnectCnt()
	{
		vector <uint> injection(n);
		Graph C = condensate(injection);
		uint cn = C.getN();
		vector <bool> source(cn, true);
		for (uint v = 0; v < cn; v++)
		{
			IterPtr iter = (C.getIncidents(v)).getIterator();
			while (!iter->isEnd())
			{
				int u = iter->getCurrentVertex();
				source[u] = false;
				//	sourcesCnt--;
				iter->next();
			}
		}
		int sourceCnt = 0;
		int sinkCnt = 0;
		int isolated = 0;
		for (uint v = 0; v < cn; v++)
		{
			bool sinkFlag = C.getIncidents(v).getIterator()->isEnd();
			if (source[v])
				sourceCnt++;
			if (sinkFlag)
				sinkCnt++;
		}
		return max(sinkCnt, sourceCnt);
	}

	void connectivityProblem(vector <pair <uint, uint> > & edgesToAdd)
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
			IterPtr iter = (C.getIncidents(v)).getIterator();
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
			sink[v] = C.getIncidents(v).getIterator()->isEnd();
			if (sink[v])
				sinksCnt++;
		}
		//optimalAnswer = max(sourcesCnt, sinksCnt);
		vector <uint> sourcesList, sinksList;
		GraphAlgo CAlgo(&C);
		for (uint v = 0; v < cn; v++) if (!mark[v] && source[v])
		{
			int w = CAlgo.findSink(v, mark);
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

	bool findCycles() const
	{
		vector <bool> mark(n, false);
		for (uint v = 0; v < n; v++)
			if (!mark[v])
				if (cycleFinding(v, -1, mark))
					return true;
		return false;
	}

	void dfs(const uint v, vector <bool> & was, vector <uint> & order) const
	{
		was[v] = true;
		IterPtr iterPtr = (getInc(v)).getIterator();
		while (!iterPtr->isEnd())
		{
			uint u = iterPtr->getCurrentVertex();
			if (!was[u])
				dfs(u, was, order);
			iterPtr->next();
		}
		order.push_back(v);
	}



private:
	Graph * G;
	uint n;

	AbstractLI & getInc(uint v) const
	{
		return G->getIncidents(v);
	}

	void tarjanAlgo(const uint v, vector <int> & status, stack <int> & st, vector <int> & up, vector <int> & inTime, uint & curTime,
		uint & curColor, vector <uint> & injection) const
	{
		status[v] = GREY;
		st.push(v);
		curTime++;
		up[v] = inTime[v] = curTime;
		IterPtr iterPtr = (getInc(v)).getIterator();
		while (!iterPtr->isEnd())
		{
			uint u = iterPtr->getCurrentVertex();
			if (status[u] == WHITE)
			{
				tarjanAlgo(u, status, st, up, inTime, curTime, curColor, injection);
				up[v] = min(up[v], up[u]);
			}
			else if (status[u] == GREY)
				up[v] = min(up[v], inTime[u]);
			iterPtr->next();
		}
		if (up[v] == inTime[v])
		{
			int cur;
			do
			{
				cur = st.top();
				st.pop();
				injection[cur] = curColor;
				status[cur] = BLACK;
			} while (cur != v);
			curColor++;
		}
	}

	int findSink(const uint v, vector <bool> & mark) const
	{
		mark[v] = true;
		if (getInc(v).getIterator()->isEnd())
			return v;
		IterPtr iterPtr = (getInc(v)).getIterator();
		while (!iterPtr->isEnd())
		{
			int u = iterPtr->getCurrentVertex();
			if (!mark[u])
			{
				int tmp = findSink(u, mark);
				if (tmp != -1)
					return tmp;
			}
			iterPtr->next();
		}
		return -1;
	}

	bool cycleFinding(const int v, const int anc, vector <bool> & mark) const
	{
		mark[v] = true;
		IterPtr iterPtr = (getInc(v)).getIterator();
		while (!iterPtr->isEnd())
		{
			int u = iterPtr->getCurrentVertex();
			if (u == anc && mark[u])
				return true;
			if (!mark[u])
			{
				if (cycleFinding(u, v, mark))
					return true;
			}
			iterPtr->next();
		}
		return false;
	}
};