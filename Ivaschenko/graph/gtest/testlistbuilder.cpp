#include <gtest/gtest.h>

#include <iostream>
#include <numeric>
#include <vector>

#include "lists/consecutiveincidencelist.h"
#include "lists/vectorincidencelist.h"
#include "lists/bitsetincidencelist.h"
#include "lists/emptyincidencelist.h"
#include "lists/setincidencelist.h"
#include "lists/singlevertexlist.h"

#include "fabrics/defaultlistbuilder.h"

#include "graph.h"

typedef enum
{
	VECTOR_LIST,
	BITSET_LIST,
	SET_LIST,
	EMPTY_LIST,
	SINGLE_VERTEX_LIST,
	CONSECUTIVE_LIST
} ListType;

struct TestCase
{
	std::vector< std::vector<graph::vertex_t> > adjList;
	std::vector< std::vector<graph::vertex_t> > backList;
	std::size_t n;

	TestCase(std::size_t _n, const std::vector< std::pair<int, int> > &edges): n(_n)
	{
		adjList.resize(n);
		for (std::pair<int, int> e : edges)
		{
			adjList[e.first].push_back(e.second);
			backList[e.second].push_back(e.first);
		}
	}

	TestCase(std::size_t _n, std::size_t m, std::size_t seed): n(_n)
	{
		std::mt19937 generator(seed);
		adjList.resize(n);
		for (size_t i = 0; i < m; ++i)
		{
			std::size_t a = generator() % n, b = generator() % n;
			adjList[a].push_back(b);
			backList[b].push_back(a);
		}
	}

	bool connected(int u, int v)
	{
		return std::find(adjList[u].begin(), adjList[u].end(), v) != adjList[u].end();
	}

	ListType preferredListType(std::size_t v)
	{

	}

	ListType preferredBackListType(std::size_t v)
	{

	}
};

void checkType(const graph::IncidenceList* list, std::size_t type)
{
	switch (type)
	{
		case VECTOR_LIST:
			EXPECT_NE(dynamic_cast<const graph::VectorIncidenceList*>(list), nullptr);
		break;
		case BITSET_LIST:
			EXPECT_NE(dynamic_cast<const graph::BitsetIncidenceList*>(list), nullptr);
		break;
		case SET_LIST:
			EXPECT_NE(dynamic_cast<const graph::SetIncidenceList*>(list), nullptr);
		break;
		case EMPTY_LIST:
			EXPECT_NE(dynamic_cast<const graph::EmptyIncidenceList*>(list), nullptr);
		break;
		case SINGLE_VERTEX_LIST:
			EXPECT_NE(dynamic_cast<const graph::SingleVertexList*>(list), nullptr);
		break;
		case CONSECUTIVE_LIST:
			EXPECT_NE(dynamic_cast<const graph::ConsecutiveIncidenceList*>(list), nullptr);
		break;
	}
}

TEST(DefaultListBuilder, ListTypeSensibilityTest)
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

	std::vector< std::unique_ptr<graph::IncidenceList> > lists, backLists;
	for (auto test : tests)
	{
		for (graph::vertex_t v = 0; v < test.n; ++v)
		{
			{
				graph::DefaultListBuilder builder(test.n);
				for (graph::vertex_t to : test.adjList[v])
					builder.addEdge(to);
				lists.push_back(builder.getList());
			}
			{
				graph::DefaultListBuilder builder(test.n);
				for (graph::vertex_t to : test.backList[v])
					builder.addEdge(to);
				lists.push_back(builder.getList());
			}
		}
		graph::Graph g(lists);
		ASSERT_EQ(g.size(), test.n);
		for (std::size_t i = 0; i < test.n; ++i)
			checkType(g.getEdgesFrom(i), test.preferredListType(i));
		for (std::size_t i = 0; i < test.n; ++i)
			checkType(g.getEdgesTo(i), test.preferredBackListType(i));
		for (std::size_t v = 0; v < test.n; ++v)
		{
			for (auto it = g.getEdgesFrom(v)->getIterator(); it->valid(); it->moveForward())
				ASSERT_TRUE(test.connected(v, it->getVertex()));
			for (auto it = g.getEdgesTo(v)->getIterator(); it->valid(); it->moveForward())
				ASSERT_TRUE(test.connected(it->getVertex(), v));
		}
		for (std::size_t v = 0; v < test.n; ++v)
			for (auto to : test.adjList[v])
			{
				ASSERT_TRUE(g.getEdgesFrom(v)->connected(to));
				ASSERT_TRUE(g.getEdgesTo(to)->connected(v));
			}
	}
}
