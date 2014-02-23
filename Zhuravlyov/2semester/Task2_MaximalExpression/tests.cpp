#include "solutions.h"
#include "gtest/gtest.h"
#include <vector>

TEST(ManualTests, DpTesting)
{
	int a[4] = {0, -1, -1, 3};
	std::vector<long long> data(a, a+4);
	EXPECT_EQ(get_result(data), 4);
}

TEST(ManualTests, StupudTesting)
{
	int a[4] = {6, 2, -14, 16};
	std::vector<long long> data(a, a+4);
	EXPECT_EQ(get_result_slow(data), 14);
}

TEST(StressTests, MainStressTest)
{
	const int number_of_tests = 20;
	const int size_of_data = 20;
	const int range = 10;
	std::vector<long long> a(size_of_data);
	for (int j = 0; j < number_of_tests; j++)
	{
		for (int i = 0; i < size_of_data; i++)
			a[i] = rand() % range - range/2;
		EXPECT_EQ(get_result_slow(a), get_result(a));
	}
}