#include "gtest/gtest.h"
#include "problem.h"
#include "testsupply.h"
#include <string>

TEST(ManualTests, SameStrings)
{
	std::string a, b;
	a = b = "ababcaba";
	std::vector<std::string> result;
	get_answer(a, b, result);
	EXPECT_EQ(result.size(), 0);
}

TEST(ManualTests, SingleEmplace)
{
	std::string a,b;
	a = "abac";
	b = "acac";
	std::vector<std::string> result;
	get_answer(a, b, result);
	EXPECT_EQ(result.size(), 1);
	EXPECT_EQ(result[0], "change 1 symbol to b");
}

TEST(ManualTests, SingleInsertion)
{
	std::string a,b;
	a = b = "aba";
	b.erase(1, 1);
	std::vector<std::string> result;
	get_answer(a, b, result);
	EXPECT_EQ(result.size(), 1);
	EXPECT_EQ(result[0], "insert b to 1 position");
}

TEST(ManualTests, SingleDeletion)
{
	std::string a,b;
	a = b = "abacaba";
	a.erase(3, 1);
	std::vector<std::string> result;
	get_answer(a, b, result);
	EXPECT_EQ(result.size(), 1);
	EXPECT_EQ(result[0], "delete 3 symbol");
}

TEST(ManualTests, AllOperations)
{
	std::string a, b;
	a = "ACGTACGTACGT";
	b = "AGTACCTACCGT";
	std::vector<std::string> result;
	get_answer(a, b, result);
	EXPECT_EQ(result.size(), 3);
	EXPECT_EQ(result[0], "delete 9 symbol");
	EXPECT_EQ(result[1], "change 5 symbol to G");
	EXPECT_EQ(result[2], "insert C to 1 position");
}

TEST(StressTest, MainStressTest)
{
	const int number_of_tests = 1;
	const int max_string_length = 100000;
	std::string s, t;
	for (int i = 0; i < number_of_tests; i++)
	{
		s = random_string_generator(rand() % max_string_length + 1);
		t = random_string_generator(rand() % max_string_length + 1);
		std::vector<std::string> result;
		get_answer(s, t, result);
		EXPECT_TRUE( test_checker(s,t,result) );
	}
}