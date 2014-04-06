#include <vector>

#include <gtest/gtest.h>

#include "lists/vectorincidencelist.h"

typedef std::vector<graph::vertex_t> TestCase;

TestCase genTestCase(std::size_t n, std::size_t seed)
{
	std::mt19937 generator(seed);
	TestCase test(n);
	std::generate_n(test.begin(), n, generator);
	return test;
}

TEST(VectorIncidenceList, CustomTest)
{
	std::vector<TestCase> tests =
	{
		{},
		{100, 0},
		{0, 1, 2, 3, 4, 5, 6},
		{0, 100},
		{5, 3, 1},
		{1, 5, 3},
		{0, 0, 0, 3, 3, 1, 1, 1, 2, 2, 1, 3},
		genTestCase(10, 1),
		genTestCase(15, 2),
		genTestCase(100, 3)
	};

	for (TestCase test : tests)
	{
		TestCase v = test, listCopy;
		std::unique_ptr<graph::IncidenceList> list(
					new graph::VectorIncidenceList(v.begin(), v.end()));
		sort(v.begin(), v.end());
		for (auto it = list->getIterator(); it->valid(); it->moveForward())
			listCopy.push_back(it->getVertex());
		ASSERT_EQ(v, listCopy);
		for (graph::vertex_t to : test)
			ASSERT_TRUE(list->connected(to));
	}
}
