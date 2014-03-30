#include <iostream>
#include <numeric>
#include <vector>

#include <gtest/gtest.h>

#include "lists/setincidencelist.h"
#include "lists/bitsetincidencelist.h"
#include "lists/vectorincidencelist.h"
#include "lists/singlevertexlist.h"
#include "lists/emptyincidencelist.h"

#include "fabrics/vectorlistgraphfabric.h"
#include "fabrics/bitsetgraphfabric.h"
#include "fabrics/functionalgraphfabric.h"
#include "fabrics/fullgraphfabric.h"
#include "fabrics/kheapgraphfabric.h"

#include "graph.h"

struct TestCase
{
	std::vector< std::vector<graph::vertex_t> > adjList;
	std::size_t n;

	TestCase(std::size_t _n, const std::vector< std::pair<int, int> > &edges): n(_n)
	{
		adjList.resize(n);
		for (std::pair<int, int> e : edges)
			adjList[e.first].push_back(e.second);
	}

	TestCase(std::size_t _n, std::size_t m, std::size_t seed): n(_n)
	{
		std::mt19937 generator(seed);
		adjList.resize(n);
		for (size_t i = 0; i < m; ++i)
		{
			std::size_t a = generator() % n, b = generator() % n;
			adjList[a].push_back(b);
		}
	}

	bool connected(int u, int v)
	{
		return std::find(adjList[u].begin(), adjList[u].end(), v) != adjList[u].end();
	}
};

TEST(Graph, ConstuctionFromBitsetFabric)
{
	std::vector<TestCase> tests =
	{
		TestCase(3, {{0, 1}, {1, 2}, {0, 2}, {1, 0}}),
		TestCase(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {2, 0}}),
		TestCase(3, 9, 1),
		TestCase(5, 10, 2),
		TestCase(5, 10, 3),
		TestCase(10, 1000, 4),
		TestCase(20, 100, 5),
		TestCase(30, 10, 6),
		TestCase(40, 1000, 7),
		TestCase(50, 2000, 8)
	};

	for (auto test : tests)
	{
		std::unique_ptr<graph::AbstractGraphFabric> fabric(new graph::BitsetGraphFabric(test.adjList, true));
		graph::Graph g(fabric, test.n, true);
		ASSERT_EQ(g.size(), test.n);
		for (std::size_t v = 0; v < g.size(); ++v)
		{
			for (auto it = g.getEdgesFrom(v)->getIterator(); it->valid(); it->moveForward())
				ASSERT_TRUE(test.connected(v, it->getVertex()));
			for (auto it = g.getEdgesTo(v)->getIterator(); it->valid(); it->moveForward())
				ASSERT_TRUE(test.connected(it->getVertex(), v));
		}
		for (std::size_t v = 0; v < test.n; ++v)
			for (std::size_t to : test.adjList[v])
			{
				ASSERT_TRUE(g.getEdgesFrom(v)->connected(to));
				ASSERT_TRUE(g.getEdgesTo(to)->connected(v));
			}
	}
}

std::vector<std::size_t> genTest(std::size_t n, std::size_t seed)
{
	std::mt19937 generator(seed);
	std::vector<std::size_t> v(n);
	std::generate_n(v.begin(), n, generator);
	for (std::size_t i = 0; i < n; ++i) v[i] %= n;
	return v;
}

TEST(Graph, ConstuctionFromFunctionalFabric)
{
	typedef std::vector<std::size_t> TestCase;

	std::vector<TestCase> tests =
	{
		{0, 1, 2, 3, 4},
		{1, 2, 3, 4, 0},
		{1, 0, 4, 2, 3},
		genTest(10, 1),
		genTest(10, 2),
		genTest(15, 3),
		genTest(30, 4),
		genTest(50, 5)
	};

	for (TestCase test : tests)
	{
		std::unique_ptr<graph::AbstractGraphFabric> fabric(new graph::FunctionalGraphFabric(test, true));
		graph::Graph g(fabric, test.size(), true);
		ASSERT_EQ(g.size(), test.size());
		for (std::size_t v = 0; v < g.size(); ++v)
		{
			for (auto it = g.getEdgesFrom(v)->getIterator(); it->valid(); it->moveForward())
				ASSERT_TRUE(test[v] == it->getVertex());
			for (auto it = g.getEdgesTo(v)->getIterator(); it->valid(); it->moveForward())
				ASSERT_TRUE(test[it->getVertex()] == v);
		}
		for (std::size_t v = 0; v < test.size(); ++v)
		{
			ASSERT_TRUE(g.getEdgesFrom(v)->connected(test[v]));
			ASSERT_TRUE(g.getEdgesTo(test[v])->connected(v));
		}
	}
}

TEST(Graph, ConstuctionFromFullGraphFabric)
{
	typedef std::size_t TestCase;

	std::vector<TestCase> tests = {1, 2, 3, 4, 5, 6, 7};

	for (TestCase test : tests)
	{
		std::unique_ptr<graph::AbstractGraphFabric> fabric(new graph::FullGraphFabric(test));
		graph::Graph g(fabric, test, true);
		for (std::size_t u = 0; u < test; ++u)
			for (std::size_t v = 0; v < test; ++v)
			{
				ASSERT_TRUE(g.getEdgesFrom(u)->connected(v));
				ASSERT_TRUE(g.getEdgesTo(v)->connected(u));
			}
	}
}

TEST(Graph, ConstuctionFromKHeapFabric)
{
	struct TestCase
	{
		std::size_t n, k;

		TestCase(std::size_t _n, std::size_t _k): n(_n), k(_k) {}
	};

	std::vector<TestCase> tests =
	{
		TestCase(10, 1),
		TestCase(5, 2),
		TestCase(10, 3),
		TestCase(127, 2),
		TestCase(124, 5),
		TestCase(80, 3),
		TestCase(79, 3),
		TestCase(81, 3)
	};

	for (TestCase test : tests)
	{
		std::unique_ptr<graph::AbstractGraphFabric> fabric(
					new graph::KHeapGraphFabric(test.n, test.k));
		graph::Graph g(fabric, test.n, true);
		for (std::size_t v = 0; v < g.size(); ++v)
		{
			for (auto it = g.getEdgesFrom(v)->getIterator(); it->valid(); it->moveForward())
			{
				int x = it->getVertex() - v * test.k;
				ASSERT_TRUE(1 <= x && x <= (int) test.k);
			}
			for (auto it = g.getEdgesTo(v)->getIterator(); it->valid(); it->moveForward())
				ASSERT_TRUE((v - 1) / test.k == it->getVertex());
		}
		for (std::size_t v = 0; v < test.n; ++v)
			for (std::size_t x = 1; x <= test.k; ++x)
			{
				if (v * test.k + x >= test.n) continue;
				ASSERT_TRUE(g.getEdgesFrom(v)->connected(v * test.k + x));
				ASSERT_TRUE(g.getEdgesTo(v * test.k + x)->connected(v));
			}
	}
}
