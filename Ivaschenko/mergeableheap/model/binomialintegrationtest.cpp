#include <gtest/gtest.h>

#include "model/binomialheap.h"

#include <algorithm>
#include <utility>
#include <random>
#include <vector>
#include <queue>

// TODO: integration tests

TEST(BinomialHeapIntegration, HeapHandIntSort)
{
	std::vector< std::vector<int> > tests =
	{
		{3, 0, 2, 7, 5, 8, 4, 6, 1, 9},
		{0, 0, 8},
		{3, 2, 1},
		{8, 1, 94},
		{5, 4, 3, 2, 1},
		{1, 2, 3, 4, 5, 6},
		{8, -1, -35, 9, 24, 82, -59, 95}
	};
	for (auto test : tests)
	{
		BinomialHeap<int, std::less<int> > heap;

		std::vector<int> sorted = test;
		std::sort(sorted.begin(), sorted.end());

		for (int x : test)
			heap.push(x);
		for (size_t i = 0; i < sorted.size(); ++i)
			EXPECT_EQ(sorted[i], heap.pop()) << "Differs at token #" << i;
	}
}

TEST(BinomialHeapIntegration, HeapBigPermutationRevSort)
{
	std::vector<int> tests = {5, 10, 15, 20, 100, 200, 500, 1000};
	for (size_t i = 0; i < tests.size(); ++i)
	{
		std::mt19937 generator(i);
		std::vector<int> perm(tests[i]);
		for (size_t j = 0; j < perm.size(); ++j) perm[j] = j;
		std::shuffle(perm.begin(), perm.end(), generator);

		BinomialHeap<int, std::greater<int> > heap;
		for (int x : perm) heap.push(x);
		std::sort(perm.begin(), perm.end(), std::greater<int>());
		for (size_t j = 0; j < perm.size(); ++j)
			EXPECT_EQ(perm[j], heap.pop()) << "Differs at token #" << j << " in test " << i;
	}
}

TEST(BinomialHeapIntegration, StressWithPriorityQueue)
{
	struct TestCase
	{
		int queries;
		std::mt19937 generator;

		TestCase(int seed, int nqueries): queries(nqueries), generator(seed) {}

		std::pair<int, int> genQuery()
		{
			return std::make_pair(generator() % 3, generator() % 1000);
		}
	};

	std::vector<TestCase> tests =
	{
		TestCase(103, 1049),
		TestCase(14, 1941),
		TestCase(1984, 1984),
		TestCase(666, 66666)
	};

	for (TestCase test : tests)
	{
		BinomialHeap<int, std::greater<int> > heap;
		std::priority_queue<int> sheap;

		for (int i = 0; i < test.queries; ++i)
		{
			std::pair<int, int> q = test.genQuery();
			if (q.first == 0)
			{
				heap.push(q.second);
				sheap.push(q.second);
			}
			if (q.second == 1)
			{
				if (sheap.empty()) EXPECT_TRUE(heap.empty());
				else EXPECT_EQ(heap.top(), sheap.top());
			}
			if (q.second == 2)
			{
				if (sheap.empty()) EXPECT_TRUE(heap.empty());
				else
				{
					EXPECT_EQ(heap.pop(), sheap.top());
					sheap.pop();
				}
			}
		}
	}
}

TEST(BinomialHeapIntegration, CustomOperations)
{
	{ // Test #1
		BinomialHeap<int, std::less<int> > heap;
		std::vector<BinomialHeap<int, std::less<int> >::NodeIdType> ids;

		ids.push_back(heap.push(15));
		ids.push_back(heap.push(18));
		ids.push_back(heap.push(4));

		EXPECT_EQ(heap.pop(), 4);
		heap.erase(ids[0]);
		EXPECT_EQ(heap.top(), 18);
		ids.push_back(heap.push(17));
		EXPECT_EQ(heap.top(), 17);
		heap.decreaseKey(ids[1], 2);
		EXPECT_EQ(heap.pop(), 2);
		EXPECT_EQ(heap.pop(), 17);
	}
	{ // Test #2
		typedef std::pair<std::size_t, std::size_t> upair;
		BinomialHeap<upair, std::less<upair> > heap;
		std::vector<BinomialHeap<upair, std::less<upair> >::NodeIdType> ids;

		ids.push_back(heap.push({0, 0}));
		ids.push_back(heap.push({9223372036854775807, 0}));
		ids.push_back(heap.push({9223372036854775807, 0}));
		ids.push_back(heap.push({9223372036854775807, 0}));
		ids.push_back(heap.push({9223372036854775807, 0}));

		EXPECT_EQ(heap.pop(), upair(0, 0));
		heap.decreaseKey(ids[3], upair(9, 3));
		EXPECT_EQ(heap.pop(), upair(9, 3));
		heap.decreaseKey(ids[2], upair(2, 17));
		EXPECT_EQ(heap.pop(), upair(2, 17));
	}
}

TEST(BinomialHeapIntegration, DijkstraWithHeap)
{
	struct Edge
	{
		public:
			std::size_t from, to, w;

			explicit Edge(std::initializer_list<std::size_t> args)
			{
				from = *(args.begin());
				to = *(args.begin() + 1);
				w = *(args.begin() + 2);
			}
	};

	struct TestCase
	{
		public:
			std::size_t gSize;
			std::vector<Edge> edges;

			explicit TestCase(std::size_t nSize, const std::initializer_list<Edge> &nEdges)
				: gSize(nSize), edges(nEdges)	{}
	};

	struct TestCaseGenerator
	{
		public:
			static TestCase genTestCase(std::size_t n, std::size_t m, std::size_t w, std::size_t seed)
			{
				std::mt19937 generator(seed);
				TestCase ans(n, {});
				for (std::size_t i = 0; i < m; ++i)
					ans.edges.push_back(Edge({generator() % n, generator() % n, generator() % w}));
				return ans;
			}

			static TestCase genTestCaseFixedProb(std::size_t n, std::double_t prob, std::size_t w, std::size_t seed)
			{
				std::mt19937 generator(seed);
				TestCase ans(n, {});
				for (std::size_t i = 0; i < n; ++i)
					for (std::size_t j = 0; j < n; ++j)
						if (generator() < prob * generator.max())
							ans.edges.push_back(Edge({i, j, generator() % w}));
				return ans;
			}
	};

	struct Dijkstra
	{
		private:
			typedef std::pair<std::size_t, std::size_t> upair;
			typedef BinomialHeap<upair, std::less<upair> >::NodeIdType IdType;

		public:
			static std::vector<std::size_t> findPathes(const TestCase &g)
			{
				const std::size_t inf = std::numeric_limits<std::size_t>::max() >> 1;

				std::vector< std::vector<upair> > graph(g.gSize);
				std::vector<std::size_t> dist(g.gSize, inf);
				std::vector<IdType> ids;
				BinomialHeap< upair, std::less<upair> > heap;

				for (const Edge &e : g.edges)
					graph[e.from].push_back(std::make_pair(e.to, e.w));
				dist[0] = 0;

				for (std::size_t i = 0; i < g.gSize; ++i)
					ids.push_back(heap.push(upair(dist[i], i)));

				for (std::size_t it = 0; it < g.gSize; ++it)
				{
					upair cur = heap.pop();
					if (cur.first == inf) break;

					for (const upair &e : graph[cur.second])
						if (dist[e.first] > dist[cur.second] + e.second)
						{
							dist[e.first] = dist[cur.second] + e.second;
							heap.decreaseKey(ids[e.first], upair(dist[e.first], e.first));
						}
				}
				return dist;
			}
	};

	struct Floyd
	{
		public:
			static std::vector<std::size_t> findPathes(const TestCase &g)
			{
				const std::size_t inf = std::numeric_limits<std::size_t>::max() >> 1;

				std::vector< std::vector<std::size_t> > dist(g.gSize, std::vector<std::size_t>(g.gSize, inf));
				for (std::size_t i = 0; i < g.gSize; ++i) dist[i][i] = 0;
				for (const Edge &e : g.edges)
					dist[e.from][e.to] = std::min(dist[e.from][e.to], e.w);

				for (std::size_t k = 0; k < g.gSize; ++k)
					for (std::size_t i = 0; i < g.gSize; ++i)
						for (std::size_t j = 0; j < g.gSize; ++j)
							dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);

				return dist[0];
			}
	};

	std::vector<TestCase> tests =
	{
		TestCase(3, {Edge({0, 1, 3}), Edge({0, 2, 1}), Edge({2, 1, 1})}),
		TestCase(5, {Edge({0, 1, 10}), Edge({0, 2, 30}), Edge({0, 3, 50}), Edge({0, 4, 10}),
					 Edge({3, 1, 40}), Edge({4, 0, 10}), Edge({4, 2, 10}), Edge({4, 3, 30})}),
		TestCaseGenerator::genTestCase(5, 10, 5, 1),
		TestCaseGenerator::genTestCase(5, 11, 5, 2),
		TestCaseGenerator::genTestCaseFixedProb(5, 0.3, 10, 3),
		TestCaseGenerator::genTestCaseFixedProb(5, 0.4, 10, 4),
		TestCaseGenerator::genTestCaseFixedProb(5, 0.5, 10, 5),
		TestCaseGenerator::genTestCaseFixedProb(5, 0.8, 10, 6),
		TestCaseGenerator::genTestCaseFixedProb(10, 0.2, 10, 7),
		TestCaseGenerator::genTestCaseFixedProb(10, 0.5, 10, 8),
		TestCaseGenerator::genTestCaseFixedProb(10, 0.8, 10, 9)
	};

	for (std::size_t i = 0; i < tests.size(); ++i)
	{
		std::vector<std::size_t> output = Dijkstra::findPathes(tests[i]);
		std::vector<std::size_t> expected = Floyd::findPathes(tests[i]);
		ASSERT_EQ(output, expected) << "test # " << i << " verticies = " << tests[i].gSize
									<< " edges = " << tests[i].edges.size();
	}
}
