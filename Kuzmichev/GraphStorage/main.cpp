#include "Graph.h"
#include "Factory.h"

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

TEST(condensate_tests, handy_test)
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
	Graph C = G.condensate(injection);
	ASSERT_TRUE(C.checkConnection());
	ASSERT_EQ(C.getN(), 3);
	ASSERT_EQ(injection[0], 2);
	ASSERT_EQ(injection[1], 0);
	ASSERT_EQ(injection[2], 2);
	ASSERT_EQ(injection[3], 0);
	ASSERT_EQ(injection[4], 1);
	ASSERT_EQ(injection[5], 0);
	ASSERT_FALSE(C.getIncidents(0).isConnected(0));
	ASSERT_TRUE(C.getIncidents(0).isConnected(1));
	ASSERT_TRUE(C.getIncidents(0).isConnected(2));

	ASSERT_FALSE(C.getIncidents(1).isConnected(0));
	ASSERT_FALSE(C.getIncidents(1).isConnected(1));
	ASSERT_FALSE(C.getIncidents(1).isConnected(2));

	ASSERT_FALSE(C.getIncidents(2).isConnected(0));
	ASSERT_FALSE(C.getIncidents(2).isConnected(1));
	ASSERT_FALSE(C.getIncidents(2).isConnected(2));
}

TEST(connectivity_tests, handy_test)
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
	int optimalAnswer;
	G.connectivityProblem(addedEdges, optimalAnswer);
	//ASSERT_EQ(addedEdges.size(), G.getOptimalAnswer());
	ASSERT_EQ(addedEdges.size(), optimalAnswer);
	for (uint i = 0; i < addedEdges.size(); i++)
		fact[addedEdges[i].first].addEdge(addedEdges[i].second);
	v.clear();
	for (int j = 0; j <= 5; j++)
		v.push_back(move(fact[j].getLI()));
	Graph newG(v);
	newG.checkConnection();
}


inline void condensate_test_func(int n, int it_count)
{
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
	vector <uint> injection;
	Graph C = G.condensate(injection);
	//ASSERT_TRUE(C.checkConnection());
	vector < vector <uint>> groups(n);
	for (int v = 0; v < n; v++)
		groups[injection[v]].push_back(v);
	vector <bool> was(n, false);
	vector <uint> order;
	for (int v = 0; v < C.getN(); v++)
	{
		if (!was[groups[v][0]])
			G.go(groups[v][0], was, order);
		for (int j = 1; j < groups[v].size(); j++)
			ASSERT_TRUE(was[groups[v][j]]);
	}
}

TEST(condensate_tests, big_test_1)
{
	for (int j = 0; j < 10; j++)
		condensate_test_func(100, rand() % 10000);
	//condensate_test_func(2, 2);
}


inline void connectivity_test_func(int n, int it_count)
{
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
	int optimalAnswer;
	G.connectivityProblem(addedEdges, optimalAnswer);
	//ASSERT_EQ(addedEdges.size(), G.getOptimalAnswer());
	ASSERT_EQ(addedEdges.size(), optimalAnswer);
	//printf("%d\n", optimalAnswer);
	for (uint i = 0; i < addedEdges.size(); i++)
		fact[addedEdges[i].first].addEdge(addedEdges[i].second);
	vectorLI v;
	v.clear();
	for (int j = 0; j < n; j++)
		v.push_back(move(fact[j].getLI()));
	Graph newG(v);
	newG.checkConnection();
}

TEST(connectivity_tests, big_test_1)
{
	for (int j = 0; j < 10; j++)
		connectivity_test_func(100, rand() % 10000);
}

TEST(connectivity_tests, big_test_2)
{
	//for (int j = 0; j < 10; j++)
	connectivity_test_func(1000, rand() % 10000);
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
	factory_test_func(10, 100);
}

TEST(factory_tests, factory_test_3)
{
	factory_test_func(100, 1000);
}

TEST(factory_tests, factory_test_4)
{
	factory_test_func(1000, 10000);
}


TEST(factory_tests, factory_test_5)
{
	factory_test_func(1500, 10000);
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
	G.dfs(order);
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