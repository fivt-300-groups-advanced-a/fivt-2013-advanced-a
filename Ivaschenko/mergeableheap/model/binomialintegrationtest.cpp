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
				else EXPECT_EQ(*heap.top(), sheap.top());
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
