#include <algorithm>
#include "gtest/gtest.h"
#include "bigsort.h"

TEST(BubblesortTest, Trivial)
{
	int a[5] = {0, 1, 3, 5, 4};
	bubblesort(a, 5);
	int b[5] = {0, 1, 3, 5, 4};
	std::sort(b, b + 5);
	for (int i = 0; i < 5; ++i)
		EXPECT_EQ(a[i], b[i]);
}