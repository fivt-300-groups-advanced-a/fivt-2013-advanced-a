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

TEST(InputStreamReader, BaseStringFunctions)
{
	struct TestCase
	{
		std::vector<std::string> input;
		std::vector<std::string> answer;
		std::set<char> delimeters;

		TestCase(const std::initializer_list<std::string> &nInput,
				 const std::initializer_list<std::string> &nAnswer,
				 const std::initializer_list<char> &nDelimeters)
			: input(nInput), answer(nAnswer), delimeters(nDelimeters) {}
	};

	std::vector<TestCase> tests =
	{
		TestCase(
			{"some words separated by space", "and new lines", "", "", "", "many lines"},
			{"some", "words", "separated", "by", "space", "and", "new", "lines", "many", "lines"},
			{' ', '\n'}),

		TestCase(
			{"strange-strange delimeters&symbols!@#$%^&*rubbish*&^%$#@!                 "},
			{"strange", "strange", "delimeters", "symbols", "rubbish"},
			{'!', '@', '#', '$', '%', '^', '&', '*', '-', ' ', '\n'})
	};

	int testNumber = 0;
	for (auto test : tests)
	{
		std::istream *in = prepareInput(test.input);
		InputStreamReader<std::string> reader(*in);
		reader.setDelimeters(test.delimeters.begin(), test.delimeters.end());
		std::string token;

		for (auto it = test.answer.begin(); it != test.answer.end(); ++it)
		{
			ASSERT_TRUE(reader(token)) << "Failed to read token #" << (it - test.answer.begin()) << " in test #" << testNumber;
			EXPECT_EQ(token, *it) << "Incorrect token #" << (it - test.answer.begin()) << " in test #" << testNumber;
		}
		++testNumber;
		delete in;
	}
}
