#include "Graph.h"
#include "Factory.h"
#include "GraphAlgo.h"

#include "gtest/gtest.h"

using namespace std;

string FuncLIStr = "class FuncLI";
string EmptyLIStr = "class EmptyLI";
string BitsetLIStr = "class BitsetLI";
string StandardLIStr = "class StandardLI";

inline void check(LIPtr & p, string & s)
{
	ASSERT_EQ(p->getClass(), s);
}

TEST(handy_tests, checking_types)
{
	int n = 100;
	StandardFactory fact1(n);

	LIPtr ptr1 = fact1.getLI();
	check(ptr1, EmptyLIStr);

	ASSERT_TRUE(fact1.addEdge(1));
	LIPtr ptr2 = fact1.getLI();
	check(ptr2, FuncLIStr);
	for (int j = 2; j < n; j++)
	{
		ASSERT_TRUE(fact1.addEdge(j));
		LIPtr ptr3 = fact1.getLI();
		if (j > n / 32)
			check(ptr3, BitsetLIStr);
		else check(ptr3, StandardLIStr);
	}
}

TEST(condensate_tests, handy_test_1)
{
	vectorLI v;
	vector <StandardFactory> fact(6, StandardFactory(6));
	fact[0].addEdge(2);
	fact[1].addEdge(5);
	fact[2].addEdge(0);
	fact[3].addEdge(1);
	fact[3].addEdge(2);
	fact[5].addEdge(3);
	fact[5].addEdge(4);
	for (int j = 0; j <= 5; j++)
		v.push_back(move(fact[j].getLI()));
	Graph G(v);
	GraphAlgo GAlgo(&G);
	vector <uint> injection;
	Graph C = GAlgo.condensate(injection);
	GraphAlgo CAlgo(&C);
	//ASSERT_TRUE(CAlgo.checkConnection());
	ASSERT_EQ(C.getN(), 3);
	ASSERT_EQ(injection[0], 0);
	ASSERT_EQ(injection[1], 2);
	ASSERT_EQ(injection[2], 0);
	ASSERT_EQ(injection[3], 2);
	ASSERT_EQ(injection[4], 1);
	ASSERT_EQ(injection[5], 2);
	ASSERT_FALSE(C.getIncidents(0).isConnected(0));
	ASSERT_FALSE(C.getIncidents(0).isConnected(1));
	ASSERT_FALSE(C.getIncidents(0).isConnected(2));

	ASSERT_FALSE(C.getIncidents(1).isConnected(0));
	ASSERT_FALSE(C.getIncidents(1).isConnected(1));
	ASSERT_FALSE(C.getIncidents(1).isConnected(2));

	ASSERT_TRUE(C.getIncidents(2).isConnected(0));
	ASSERT_TRUE(C.getIncidents(2).isConnected(1));
	ASSERT_FALSE(C.getIncidents(2).isConnected(2));

	ASSERT_FALSE(CAlgo.findCycles());
}



TEST(connectivity_tests, handy_test_1)
{
	vectorLI v;
	vector <StandardFactory> fact(6, StandardFactory(6));
	fact[0].addEdge(2);
	fact[1].addEdge(5);
	fact[2].addEdge(0);
	fact[3].addEdge(1);
	fact[3].addEdge(2);
	fact[5].addEdge(3);
	fact[5].addEdge(4);
	for (int j = 0; j <= 5; j++)
		v.push_back(move(fact[j].getLI()));
	Graph G(v);
	
	vector <uint> injection;
	vector < pair <uint, uint> > addedEdges;
	GraphAlgo GAlgo(&G);
	int optimalAnswer = GAlgo.getEdgesToConnectCnt();
	
	GAlgo.connectivityProblem(addedEdges);
	
	ASSERT_EQ(addedEdges.size(), optimalAnswer);
	for (uint i = 0; i < addedEdges.size(); i++)
		fact[addedEdges[i].first].addEdge(addedEdges[i].second);
	v.clear();
	for (int j = 0; j <= 5; j++)
		v.push_back(move(fact[j].getLI()));
	Graph newG(v);
	GraphAlgo newGAlgo(&newG);
	ASSERT_TRUE(newGAlgo.checkConnection());
	
}

TEST(connectivity_tests, handy_test_2)
{
	vectorLI v;
	vector <StandardFactory> fact(6, StandardFactory(6));
	for (int j = 0; j <= 5; j++)
		v.push_back(move(fact[j].getLI()));
	Graph G(v);
	vector <uint> injection;
	vector < pair <uint, uint> > addedEdges;
	GraphAlgo GAlgo(&G);
	int optimalAnswer = GAlgo.getEdgesToConnectCnt();
	GAlgo.connectivityProblem(addedEdges);
	ASSERT_EQ(addedEdges.size(), optimalAnswer);
	for (uint i = 0; i < addedEdges.size(); i++)
		fact[addedEdges[i].first].addEdge(addedEdges[i].second);
	v.clear();
	for (int j = 0; j <= 5; j++)
		v.push_back(move(fact[j].getLI()));
	Graph newG(v);
	GraphAlgo newGAlgo(&newG);
	ASSERT_TRUE(newGAlgo.checkConnection());

}

inline void condensate_test_func(int n, int it_count)
{
	it_count = min(it_count, n * (n - 1) / 2);
	vector <StandardFactory> fact(n, StandardFactory(n));
	vector < vector <bool > > a(n);
	for (int it = 0; it < n; it++)
		a[it].assign(n, false);
	for (int it = 0; it < it_count; it++)
	{
		int u, v;
		while (true)
		{
			u = rand() % n;
			v = rand() % n;
			if (!a[u][v])
			{
				break;
			}
		}
		a[u][v] = true;
		ASSERT_TRUE(fact[u].addEdge(v));
		//edges[it] = make_pair(u, v);
	}
	vectorLI fact_li(n);
	for (int v = 0; v < n; v++)
	{
		LIPtr tmp = fact[v].getLI();
		fact_li[v] = move(tmp);
	}
	Graph G(fact_li);
	GraphAlgo GAlgo(&G);
	vector <uint> injection;
	Graph C = GAlgo.condensate(injection);
	vector < vector <uint>> groups(n);
	for (int v = 0; v < n; v++)
		groups[injection[v]].push_back(v);
	vector <bool> was(n, false);
	vector <uint> order;
	for (uint v = 0; v < C.getN(); v++)
	{
		if (!was[groups[v][0]])
			GAlgo.dfs(groups[v][0], was, order);
		for (uint j = 1; j < groups[v].size(); j++)
			ASSERT_TRUE(was[groups[v][j]]);
	}
	for (uint i = 0; i < n; i++)
	{
		uint iCond = injection[i];
		IterPtr iterPtr = G.getIncidents(i).getIterator();
		while (!iterPtr->isEnd())
		{
			uint u = iterPtr->getCurrentVertex();
			uint uCond = injection[u];
			if (uCond != iCond)
				ASSERT_TRUE(C.getIncidents(iCond).isConnected(uCond));
			iterPtr->next();
		}
	}
	GraphAlgo CAlgo(&C);
	bool cyclesFound = CAlgo.findCycles();
	ASSERT_FALSE(cyclesFound);
}

TEST(condensate_tests, medium_test_1)
{
	for (int j = 0; j < 10; j++)
		condensate_test_func(rand() % 80, rand() % 6000);
}

TEST(condensate_tests, big_test_1)
{
	for (int j = 0; j < 5; j++)
		condensate_test_func(rand() % 150, rand() % 15000);
}

TEST(condensate_tests, lots_of_small_tests)
{
	for (int j = 0; j < 100; j++)
		condensate_test_func(rand() % 10 + 1, rand() % 100);
}

TEST(condensate_tests, graphs_with_4_vertex)
{
	for (int j = 0; j < 2000; j++)
		condensate_test_func(4, rand() % 6);
}



inline void connectivity_test_func(int n, int it_count)
{
	it_count = min(it_count, n * (n - 1) / 2);
	vector <StandardFactory> fact(n, StandardFactory(n));
	//for (int i = 0; i < n; i++)
	//fact.push_back(StandardFactory(n));

	//vector < pair <int, int> > edges(it_count);
	vector < vector <bool > > a(n);
	for (int it = 0; it < n; it++)
		a[it].assign(n, false);
	for (int it = 0; it < it_count; it++)
	{
		int u, v;
		while (true)
		{
			u = rand() % n;
			v = rand() % n;
			if (!a[u][v])
			{
				break;
			}
		}
		a[u][v] = true;
		ASSERT_TRUE(fact[u].addEdge(v));
		//edges[it] = make_pair(u, v);
	}
	vectorLI fact_li(n);
	for (int v = 0; v < n; v++)
	{
		LIPtr tmp = fact[v].getLI();
		fact_li[v] = move(tmp);
	}
	Graph G(fact_li);
	vector < pair <uint, uint > > addedEdges;
	GraphAlgo GAlgo(&G);
	int optimalAnswer = GAlgo.getEdgesToConnectCnt();
	GAlgo.connectivityProblem(addedEdges);
	//ASSERT_EQ(addedEdges.size(), G.getEdgesToConnectCnt());
	ASSERT_EQ(addedEdges.size(), optimalAnswer);
	//printf("%d\n", optimalAnswer);
	for (uint i = 0; i < addedEdges.size(); i++)
		fact[addedEdges[i].first].addEdge(addedEdges[i].second);
	vectorLI v;
	for (int j = 0; j < n; j++)
		v.push_back(move(fact[j].getLI()));
	Graph newG(v);
	GraphAlgo newGAlgo(&newG);
	ASSERT_TRUE(newGAlgo.checkConnection());
}

TEST(connectivity_tests, big_test_1)
{
	for (int j = 0; j < 10; j++)
		connectivity_test_func(rand() % 150, rand() % 1500);
}

TEST(connectivity_tests, big_test_2)
{
	connectivity_test_func(1000, rand() % 10000);
}

TEST(connectivity_tests, graphs_with_almost_no_edges)
{
	for (int j = 0; j < 1000; j++)
		connectivity_test_func(rand() % 20 + 1, rand() % 4);
}

TEST(connectivity_tests, graphs_with_4_vertex)
{
	for (int j = 0; j < 2000; j++)
		connectivity_test_func(4, rand() % 6);
}

TEST(connectivity_tests, lots_of_small_tests)
{
	for (int j = 0; j < 1000; j++)
	{
		int nj = rand() % 10 + 1;
		connectivity_test_func(nj, rand() % 50);
		//connectivity_test_func(10, 50);
	}
}

TEST(factory_tests, factory_test_1)
{

	int n = 100;
	for (int i = 0; i < n; i++)
	{
		vector <bool> vb;
		vector <int> list;
		StandardFactory fact(n);
		for (int j = 0; j < n; j++)
		if (rand() % 2 == 0)
		{
			ASSERT_TRUE(fact.addEdge(j));
			vb.push_back(true);
			list.push_back(j);
		}
		else vb.push_back(false);
		LIPtr li = fact.getLI();
		for (int j = 0; j < n; j++)
		{
			bool liConnected = li->isConnected(j);
			//printf("j = %d %d ?= %d\n", j, liConnected, (int)vb[j]);
			ASSERT_EQ(liConnected, vb[j]);
		}
		IterPtr ip = li->getIterator();
		uint cur = 0;
		while (!ip->isEnd() && cur < list.size())
		{
			int x = ip->getCurrentVertex();
			int y = list[cur];
			ASSERT_EQ(x, y);
			cur++;
			ip->next();
		}
		ASSERT_TRUE(ip->isEnd());
		ASSERT_TRUE(cur == list.size());
	}

}

inline void factory_test_func(int n, int it_count)
{
	it_count = min(it_count, n * (n - 1) / 2);
	vector <StandardFactory> fact(n, StandardFactory(n));
	//for (int i = 0; i < n; i++)
		//fact.push_back(StandardFactory(n));

	vector < pair <int, int> > edges(it_count);
	vector < vector <bool > > a(n);
	for (int it = 0; it < n; it++)
		a[it].assign(n, false);
	for (int it = 0; it < it_count; it++)
	{
		int u, v;
		while (true)
		{
			u = rand() % n;
			v = rand() % n;
			if (!a[u][v])
			{
				break;
			}
		}
		a[u][v] = true;
		ASSERT_TRUE(fact[u].addEdge(v));
		edges[it] = make_pair(u, v);
	}
	vector <LIPtr> fact_li(n);
	for (int v = 0; v < n; v++)
	{
		LIPtr tmp = fact[v].getLI();
		fact_li[v] = move(tmp);
	}
	for (int it = 0; it < it_count; it++)
	{
		bool f = fact_li[edges[it].first]->isConnected(edges[it].second);
		ASSERT_TRUE(f);
	}
	for (int v = 0; v < n; v++)
	{
		vector <int> v_edges;
		for (int j = 0; j < n; j++)
			if (a[v][j])
				v_edges.push_back(j);
		IterPtr ip = fact_li[v]->getIterator();
		uint cur = 0;
		while (!ip->isEnd() && cur < v_edges.size())
		{
			int x = ip->getCurrentVertex();
			int y = v_edges[cur];
			ASSERT_EQ(x, y);
			cur++;
			ip->next();
		}
		ASSERT_TRUE(ip->isEnd());
		ASSERT_EQ(cur, v_edges.size());
	}
}

TEST(factory_tests, factory_test_2)
{
	factory_test_func(50, 200);
}

TEST(factory_tests, factory_test_3)
{
	factory_test_func(100, 1000);
}

TEST(factory_tests, factory_test_4)
{
	factory_test_func(1000, 10000);
}


TEST(factory_tests, lots_of_small_tests)
{
	for (int i = 0; i < 2000; i++)
		factory_test_func(rand() % 10, rand() % 100);
}

TEST(handy_tests, li_and_dfs_check)
{
	vectorLI v;
	vector <uint> t1;
	t1.push_back(2);

	LIPtr ptr0((AbstractLI *)(new StandardLI(t1, false)));
	v.push_back(move(ptr0));
	vector <bool> t2;
	t2.push_back(0);
	t2.push_back(0);
	t2.push_back(0);
	t2.push_back(1);
	LIPtr ptr1((AbstractLI *)(new BitsetLI(t2)));
	v.push_back(move(ptr1));

	LIPtr ptr2((AbstractLI *)(new FuncLI(4)));
	v.push_back(move(ptr2));

	vector <uint> t3;
	t3.push_back(2);
	LIPtr ptr3((AbstractLI *)(new StandardLI(t3)));
	v.push_back(move(ptr3));

	LIPtr ptr4((AbstractLI *)(new EmptyLI()));
	v.push_back(move(ptr4));

	Graph G(v);
	vector <uint> order;
	GraphAlgo GAlgo(&G);
	GAlgo.dfsFromAllVertex(order);
	ASSERT_EQ(order.size(), 5);
	ASSERT_EQ(order[0], 4);
	ASSERT_EQ(order[1], 2);
	ASSERT_EQ(order[2], 0);
	ASSERT_EQ(order[3], 3);
	ASSERT_EQ(order[4], 1);
}


int main(int argc, char ** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}