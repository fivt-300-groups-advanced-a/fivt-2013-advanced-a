#include <algorithm>
#include <vector>
#include <random>

#include <gtest/gtest.h>

#include "lists/singlevertexlist.h"

namespace
{
	struct TestCase
	{
		std::size_t v;

		TestCase(std::size_t nv): v(nv) {}

		std::size_t size() const
		{
			return 1;
		}
	};

	TEST(SingleVertexList, CustomTest)
	{
		std::vector<TestCase> tests =
		{
			TestCase(0),
			TestCase(2),
			TestCase(3),
			TestCase(4),
			TestCase(100),
			TestCase(1435),
		};

		for (TestCase test : tests)
		{
			std::unique_ptr<graph::IncidenceList> list(new graph::SingleVertexList(test.v));
			ASSERT_EQ(list->size(), test.size());
			auto it = list->getIterator();
			ASSERT_TRUE(it->valid());
			ASSERT_EQ(it->getVertex(), test.v);
			it->moveForward();
			ASSERT_FALSE(it->valid());
			ASSERT_TRUE(list->connected(test.v));
		}
	}
}
