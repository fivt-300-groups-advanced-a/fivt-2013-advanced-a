#include <gtest/gtest.h>

#include <iostream>
#include <memory>

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

struct Date
{
	unsigned int day, month, year;

	Date() {}
	Date(unsigned int nd, unsigned int nm, unsigned int ny): day(nd), month(nm), year(ny) {}

	inline bool operator == (const Date &d) const
	{
		return day == d.day && month == d.month && year == d.year;
	}
};

namespace std
{
	istream& operator >> (istream &in, Date &d)
	{
		while (true)
		{
			std::string s;
			if (!(in >> s)) break;
			if (sscanf(s.c_str(), "%u-%u-%u", &d.day, &d.month, &d.year) == 3) break;
		}
		return in;
	}
}

TEST(InputStreamReader, CustomTypeBaseFunctions)
{
	struct TestCase
	{
		std::vector<std::string> input;
		std::vector<Date> answer;

		TestCase(const std::initializer_list<std::string> &nInput, const std::initializer_list<Date> &nAns)
			:input(nInput), answer(nAns) {}
	};

	std::vector<TestCase> tests =
	{
		TestCase
		(
			{"23-10-1993 Some-strange-word 24-06-1996"},
			{Date(23, 10, 1993), Date(24, 6, 1996)}
		),
		TestCase
		(
			{"00-00-00 787-384-0000 rubbish sodom death 76-82-17"},
			{Date(0, 0, 0), Date(787, 384, 0), Date(76, 82, 17)}
		)
	};

	int currentTest = 0;
	for (auto test : tests)
	{
		std::unique_ptr<std::istream> in(prepareInput(test.input));
		InputStreamReader<Date> reader(*in);
		Date current;

		for (size_t i = 0; i < test.answer.size(); ++i)
		{
			ASSERT_TRUE(reader(current));
			EXPECT_TRUE(test.answer[i] == current);
		}
		++currentTest;
	}
}
