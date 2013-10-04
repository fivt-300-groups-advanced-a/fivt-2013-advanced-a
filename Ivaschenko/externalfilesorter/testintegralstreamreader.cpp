#include <fstream>

#include "gtest/gtest.h"
#include "io/inputstreamreader.h"

/*
 * Auxillary function to write few lines into file
 */
std::ifstream *prepareInput(const std::vector<std::string> &list)
{
	const std::string fileName = "testFile.txt";

	std::ofstream out(fileName);
	for (std::string line : list)
		out << line << '\n';
	out.close();

	return new std::ifstream(fileName);
}

/*
 * Testing basic functions of InputStreamReader
 *   - for ints
 * TODO: for float, strings and some structure
 */
TEST(InputStreamReader, BaseIntegralFunctions)
{
	std::ifstream *in = prepareInput({"    1   2 3 4 5 6 7 ", "7 6 5 4 trash 888notnumber numbernot888 3 2 1"});
	std::vector< std::vector<int> > tests = {{1, 2, 3, 4, 5, 6, 7}, {7, 6, 5, 4, 3, 2, 1}};
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
	in->close();
	delete in;
}

/*
 * Testing delimeters options of InputStreamReader (for integral types)
 * TODO: test small functions like getters, setters, etc.
 */
TEST(InputStreamReader, IntegralDelimeterTricks)
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
		TestCase({"1,2,4, 4 ,8,9"}, {1, 2, 4, 8, 9}, {','})
	};

	int currentValue, testNumber = 0;
	for (auto test : tests)
	{
		std::ifstream *in = prepareInput(test.input);
		InputStreamReader<int> reader(*in);
		int tokenNumber = 0;
		reader.setDelimeters(test.delimeters);
		for (auto jt = test.answer.begin(); jt != test.answer.end(); ++jt, ++tokenNumber)
		{
			ASSERT_TRUE(reader(currentValue)) << "Failed to read token #" << tokenNumber << " in test #" << testNumber;
			EXPECT_EQ(currentValue, *jt) << "Incorrect token #" << tokenNumber << " in test #" << testNumber;
		}
		ASSERT_FALSE(reader(currentValue));
		in->close();
		delete in;
		++testNumber;
	}
}

/*
 * Testing reading collections
 * TODO: for ints, for others
 */
TEST(InputStreamReader, CollectionReading)
{

}

/*
 * Testing InputStreamReader reading of non-decimal ints
 */
TEST(InputStreamReader, NonDecimal)
{

}
