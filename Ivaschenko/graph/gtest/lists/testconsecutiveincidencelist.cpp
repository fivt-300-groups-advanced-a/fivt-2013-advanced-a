#include <algorithm>
#include <vector>
#include <random>

#include <gtest/gtest.h>

#include "lists/consecutiveincidencelist.h"

namespace
{
	struct TestCase
	{
		std::size_t a, b;

		TestCase(std::size_t na, std::size_t nb): a(na), b(nb)
		{
			assert(a <= b);
		}

		std::size_t size() const
		{
			return b - a;
		}
	};

	TestCase genTestCase(std::size_t n, std::size_t seed)
	{
		std::mt19937 generator(seed);
		std::size_t start = generator() % 1000000000;
		return TestCase(start, start + n);
	}

	TEST(ConsecutiveIncidenceList, CustomTest)
	{
		std::vector<TestCase> tests =
		{
			TestCase(0, 3),
			TestCase(2, 5),
			TestCase(3, 3),
			TestCase(3, 4),
			TestCase(100, 105),
			TestCase(0, 0),
			genTestCase(10, 1),
			genTestCase(15, 2),
			genTestCase(100, 3),
		};

		for (TestCase test : tests)
		{
			std::unique_ptr<graph::IncidenceList> list(new graph::ConsecutiveIncidenceList(
													   test.a, test.b));
			ASSERT_EQ(list->size(), test.size());
			std::size_t cnt = 0;
			int prev = -1;
			for (auto it = list->getIterator(); it->valid(); it->moveForward())
			{
				if (prev != -1) ASSERT_EQ(it->getVertex(), prev + 1);
				++cnt;
				prev = it->getVertex();
			}
			ASSERT_EQ(cnt, test.size());
			for (std::size_t to = test.a; to < test.b; ++to)
				ASSERT_TRUE(list->connected(to));
		}
	}
}


