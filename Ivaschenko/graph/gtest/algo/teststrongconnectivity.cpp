#include <gtest/gtest.h>

#include "graph.h"

#include "algo/strongconnectivity.h"
#include "fabrics/defaultlistbuilder.h"

namespace
{
	struct TestCase
	{
		public:
			std::vector< std::vector<graph::vertex_t> > adjList;

			TestCase(std::size_t n, const std::vector< std::pair<int, int> > &edges): adjList(n)
			{
				for (auto e : edges)
					adjList[e.first].push_back(e.second);
			}

			TestCase(std::size_t n, std::size_t m, std::size_t seed): adjList(n)
			{
				std::mt19937 generator(seed);
				for (std::size_t it = 0; it < m; ++it)
					adjList[generator() % n].push_back(generator() % n);
			}

			std::size_t size() const
			{
				return adjList.size();
			}

			bool reachable(graph::vertex_t a, graph::vertex_t b) const
			{
				std::vector<bool> used(adjList.size(), false);
				return dfs(a, b, used);
			}

		private:
			bool dfs(graph::vertex_t v, graph::vertex_t u, std::vector<bool> &used) const
			{
				if (used[v]) return false;
				if (u == v) return true;
				used[v] = true;
				for (std::size_t to : adjList[v])
					if (dfs(to, u, used)) return true;
				return false;
			}
	};

	TEST(StrongConnectivity, FindComponents)
	{
		std::vector<TestCase> tests =
		{
			TestCase(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}),
			TestCase(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {0, 2}}),
			TestCase(5, {{0, 1}, {1, 0}, {2, 3}, {3, 4}, {2, 4}}),
			TestCase(5, 8, 1),
			TestCase(5, 12, 2),
			TestCase(1, 5, 3),
			TestCase(10, 0, 4),
			TestCase(10, 40, 5),
			TestCase(10, 100, 6),
			TestCase(10, 500, 7),
		};

		for (auto test : tests)
		{
			std::vector< std::unique_ptr<graph::IncidenceList> > lists(test.size());
			for (std::size_t v = 0; v < test.size(); ++v)
			{
				graph::DefaultListBuilder builder(test.size());
				for (graph::vertex_t to : test.adjList[v])
					builder.addEdge(to);
				lists[v] = builder.getList();
			}
			graph::Graph g(lists);

			std::vector<graph::vertex_t> colors = graph::strong_connectivity::findComponents(g);
			bool strongConnected = true;
			for (std::size_t v = 0; v < test.size(); ++v)
				for (std::size_t u = v; u < test.size(); ++u)
				{
					bool direct = test.reachable(v, u), rev = test.reachable(u, v);
					strongConnected &= direct && rev;
					EXPECT_EQ(direct && rev, colors[u] == colors[v]) << "error on (" << v << ", " << u << ")";
				}
			EXPECT_EQ(strongConnected, graph::strong_connectivity::isStrongConnected(g));
		}
	}

	TEST(StrongConnectivity, FindAugmentation)
	{
		std::vector<TestCase> tests =
		{
			TestCase(5, {{0, 1}, {1, 0}, {2, 3}, {3, 4}, {2, 4}}),
			TestCase(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}}),
			TestCase(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {0, 2}}),
			TestCase(5, 8, 1),
			TestCase(5, 12, 2),
			TestCase(1, 5, 3),
			TestCase(10, 0, 4),
			TestCase(10, 40, 5),
			TestCase(10, 100, 6),
			TestCase(10, 500, 7),
		};
		for (auto test : tests)
		{
			std::vector< std::unique_ptr<graph::IncidenceList> > lists(test.size());
			for (std::size_t v = 0; v < test.size(); ++v)
			{
				graph::DefaultListBuilder builder(test.size());
				for (graph::vertex_t to : test.adjList[v])
					builder.addEdge(to);
				lists[v] = std::move(builder.getList());
			}
			graph::Graph g(lists);

			auto added = graph::strong_connectivity::strongConnectivityAugmentation(g);

			std::vector<graph::DefaultListBuilder> builders(test.size(), graph::DefaultListBuilder(test.size()));
			for (graph::vertex_t v = 0; v < test.size(); ++v)
				for (graph::vertex_t to : test.adjList[v])
					builders[v].addEdge(to);
			for (auto e : added)
				builders[e.first].addEdge(e.second);
			lists.clear();
			for (graph::vertex_t v = 0; v < test.size(); ++v)
				lists.push_back(std::move(builders[v].getList()));

			graph::Graph augmented(lists);
			ASSERT_TRUE(graph::strong_connectivity::isStrongConnected(augmented));
		}
	}
}
