#include <algorithm>
#include <vector>
#include <random>

#include <gtest/gtest.h>

#include "lists/emptyincidencelist.h"

namespace
{
	TEST(EmptyIncidenceList, CustomTest)
	{
		std::unique_ptr<graph::IncidenceList> list(new graph::EmptyIncidenceList());
		ASSERT_EQ(list->size(), 0);
		auto it = list->getIterator();
		ASSERT_FALSE(it->valid());
		ASSERT_FALSE(list->connected(0));
		ASSERT_FALSE(list->connected(2958));
	}
}

