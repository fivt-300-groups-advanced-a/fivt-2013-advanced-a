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
	vector <StandardFactory> fact;
	for (int i = 0; i < n; i++)
		fact.push_back(StandardFactory(n));

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
	vector <int> order;
	G.dfs(order);
	ASSERT_EQ(order.size(), 5);
	ASSERT_EQ(order[0], 0);
	ASSERT_EQ(order[1], 2);
	ASSERT_EQ(order[2], 4);
	ASSERT_EQ(order[3], 1);
	ASSERT_EQ(order[4], 3);
	
	
}

int main(int argc, char ** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}