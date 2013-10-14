#include <fstream>

#include <gtest/gtest.h>

#include "io/inputstreamreader.h"

namespace
{
	/*
	 * Returns stream, containing some lines of text
	 */
	std::istream *prepareInput(const std::vector<std::string> &list)
	{
		std::string rawData = "";
		for (auto line : list) rawData += line + '\n';
		return new std::istringstream(rawData);
	}
}

/*
 * Testing basic functions of InputStreamReader
 *   - for ints
 * TODO: for float, strings and some structure
 */
TEST(InputStreamReader, BaseIntegerFunctions)
{
	std::istream *in = prepareInput({"    1   -2 3 4 5 6 -7 ", "7 -6 5 4 trash 888notnumber numbernot888 3 2 1 -172-"});
	std::vector< std::vector<int> > tests = {{1, -2, 3, 4, 5, 6, -7}, {7, -6, 5, 4, 3, 2, 1}};
	InputStreamReader<int> reader(*in);

	int currentValue, lineNumber = 0;
	for (auto it = tests.begin(); it != tests.end(); ++it, ++lineNumber)
	{
		int tokenNumber = 0;
		for (auto jt = it->begin(); jt != it->end(); ++jt, ++tokenNumber)
		{
			ASSERT_TRUE(reader(currentValue)) << "Failed to read token #" << tokenNumber << " in line #" << lineNumber;
			EXPECT_EQ(currentValue, *jt) << "Incorrect token #" << tokenNumber << " in line #" << lineNumber;
		}
	}

	ASSERT_FALSE(reader(currentValue));
	delete in;
}

/*
 * Testing delimeters options of InputStreamReader (for integral types)
 * TODO: test small functions like getters, setters, etc.
 */
TEST(InputStreamReader, IntegerDelimeterTricks)
{
	struct TestCase
	{
		std::vector<std::string> input;
		std::vector<int> answer;
		std::set<char> delimeters;

		TestCase(const std::initializer_list<std::string> &nInput, const std::vector<int> &nAns, const std::set<char> &nDel)
			: input(nInput), answer(nAns), delimeters(nDel) {}
	};

	std::vector<TestCase> tests =
	{
		TestCase({"1,2,-4, -4 ,8,-9"}, {1, 2, -4, 8, -9}, {',', '\n'})
	};

	int currentValue, testNumber = 0;
	for (auto test : tests)
	{
		std::istream *in = prepareInput(test.input);
		InputStreamReader<int> reader(*in);
		int tokenNumber = 0;
		reader.setDelimeters(test.delimeters.begin(), test.delimeters.end());
		for (auto jt = test.answer.begin(); jt != test.answer.end(); ++jt, ++tokenNumber)
		{
			ASSERT_TRUE(reader(currentValue)) << "Failed to read token #" << tokenNumber << " in test #" << testNumber;
			EXPECT_EQ(currentValue, *jt) << "Incorrect token #" << tokenNumber << " in test #" << testNumber;
		}
		ASSERT_FALSE(reader(currentValue));
		delete in;
		++testNumber;
	}
}

/*
 * Testing reading collections
 */
TEST(InputStreamReader, IntegerCollectionReading)
{
	struct TestCase
	{
		std::vector<std::string> input;
		std::vector<int> answer;
		std::set<char> delimeters;

		TestCase(const std::initializer_list<std::string> &nInput, const std::vector<int> &nAns, const std::set<char> &nDel)
			: input(nInput), answer(nAns), delimeters(nDel) {}
	};

	std::vector<TestCase> tests =
	{
		TestCase({"{24598, 384, -49858, -194};"}, {24598, 384, -49858, -194}, {'{', ',', ' ', '\n', '}'}),
		TestCase({"484 8918 98539853 1293 -1"}, {484, 8918, 98539853, 1293, -1}, {' ', '\n'})
	};

	int testNumber = 0;
	for (auto test : tests)
	{
		std::istream *in = prepareInput(test.input);
		InputStreamReader<int> reader(*in);
		reader.setDelimeters(test.delimeters.begin(), test.delimeters.end());
		std::vector<int> result(test.answer.size());

		ASSERT_TRUE(reader(result.begin(), result.end())) << "Failed to read input of test #" << testNumber;
		EXPECT_EQ(result, test.answer) << "Incorrect answer on test #" << testNumber;

		delete in;
	}
}

/*
 * Testing InputStreamReader reading of non-decimal ints
 */
TEST(InputStreamReader, NonDecimalIntegers)
{
	std::istream *in = prepareInput({"A AB -10C DEADBEEF", "A1A -6 5 4 trash AAAnotnumber numbernotAAA 3 2 1 -1A2-"});
	std::vector< std::vector<int> > tests = {{0xA, 0xAB, -0x10C, 0xDEADBEEF}, {0xA1A, -0x6, 5, 4, 3, 2, 1}};
	InputStreamReader<int> reader(*in);

	reader.setRadix(16);
	EXPECT_EQ(reader.getRadix(), 16);

	int currentValue, lineNumber = 0;
	for (auto it = tests.begin(); it != tests.end(); ++it, ++lineNumber)
	{
		int tokenNumber = 0;
		for (auto jt = it->begin(); jt != it->end(); ++jt, ++tokenNumber)
		{
			ASSERT_TRUE(reader(currentValue)) << "Failed to read token #" << tokenNumber << " in line #" << lineNumber;
			EXPECT_EQ(currentValue, *jt) << "Incorrect token #" << tokenNumber << " in line #" << lineNumber;
		}
	}

	ASSERT_FALSE(reader(currentValue));
	delete in;
}

/*
 * Reading file with lot of integers
 */
TEST(InputStreamReader, LotOfIntegers)
{
	const int numbers = 1000000, seed = 19482;

	srand(seed);
	std::ofstream out("testFile.txt");
	std::vector<int> answer, result(numbers);
	for (int i = 0; i < numbers; i++)
	{
		answer.push_back(rand() - RAND_MAX / 2);
		out << answer.back() << " ";
	}
	out << std::endl;
	out.close();

	std::clock_t start = std::clock();
	std::ifstream in("testFile.txt");
	InputStreamReader<int> reader(in);
	ASSERT_TRUE(reader(result.begin(), result.end()));
	EXPECT_EQ(result, answer);
	in.close();
	std::cout << "Reading finished in " << (clock() - start) / (CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
}
