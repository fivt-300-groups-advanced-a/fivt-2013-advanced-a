#include <algorithm>
#include <vector>
#include <random>

#include <gtest/gtest.h>

#include "lists/bitsetincidencelist.h"

namespace
{
	struct TestCase
	{
		std::size_t n;
		std::vector<graph::vertex_t> adjList;

		TestCase(std::size_t _n): n(_n) {}
		TestCase(std::size_t _n, const std::initializer_list<graph::vertex_t> &lst):
			n(_n), adjList(lst) {}

		std::size_t size() const
		{
			std::vector<graph::vertex_t> v = adjList;
			std::sort(v.begin(), v.end());
			return std::unique(v.begin(), v.end()) - v.begin();
		}
	};

	TestCase genTestCase(std::size_t n, std::size_t m, std::size_t seed)
	{
		std::mt19937 generator(seed);
		TestCase test(n);
		test.adjList.resize(m);
		std::generate_n(test.adjList.begin(), m, generator);
		for (std::size_t i = 0; i < m; ++i)
			test.adjList[i] %= n;
		return test;
	}

	TEST(BitsetIncidenceList, CustomTest)
	{
		std::vector<TestCase> tests =
		{
			TestCase(1, {}),
			TestCase(2, {1}),
			TestCase(3, {0, 1, 2}),
			TestCase(4, {}),
			TestCase(5, {0, 2, 2, 4}),
			TestCase(10, {0, 1, 9, 8}),
			genTestCase(10, 5, 1),
			genTestCase(15, 10, 2),
			genTestCase(100, 500, 3),
		};

		for (TestCase test : tests)
		{
			std::unique_ptr<graph::IncidenceList> list(new graph::BitsetIncidenceList(test.n,
													   test.adjList.begin(), test.adjList.end()));
			ASSERT_EQ(list->size(), test.size());
			for (auto it = list->getIterator(); it->valid(); it->moveForward())
				ASSERT_NE(std::find(test.adjList.begin(), test.adjList.end(), it->getVertex()),
						  test.adjList.end());
			for (graph::vertex_t to : test.adjList)
				ASSERT_TRUE(list->connected(to));
		}
	}
}
