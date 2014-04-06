#include <algorithm>
#include <vector>
#include <random>

#include <gtest/gtest.h>

#include "lists/setincidencelist.h"

namespace
{
	struct TestCase
	{
		std::size_t n;
		std::vector<graph::vertex_t> adjList;

		TestCase(std::size_t _n): n(_n) {}
		TestCase(std::size_t _n, const std::initializer_list<graph::vertex_t> &lst):
			n(_n), adjList(lst) {}
	};

	TestCase genTestCase(std::size_t n, std::size_t seed)
	{
		std::mt19937 generator(seed);
		TestCase test(n);
		test.adjList.resize(n);
		std::generate_n(test.adjList.begin(), n, generator);
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
			TestCase(5, {0, 2, 4}),
			TestCase(10, {0, 1, 9, 10}),
			genTestCase(10, 1),
			genTestCase(15, 2),
			genTestCase(100, 3),
		};

		for (TestCase test : tests)
		{
			std::unique_ptr<graph::IncidenceList> list(new graph::SetIncidenceList(
													   test.adjList.begin(), test.adjList.end()));
			for (auto it = list->getIterator(); it->valid(); it->moveForward())
				ASSERT_NE(std::find(test.adjList.begin(), test.adjList.end(), it->getVertex()),
						  test.adjList.end());
			for (graph::vertex_t to : test.adjList)
				ASSERT_TRUE(list->connected(to));
		}
	}
}

