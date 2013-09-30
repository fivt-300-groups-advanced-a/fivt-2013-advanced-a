#include <typeinfo>
#include <iostream>
#include <fstream>
#include <set>

#include "io/inputstreamreader.h"
#include "io/outputstreamwriter.h"
#include "io/binaryfilereader.h"
#include "io/binaryfilewriter.h"

#include <gtest/gtest.h>

namespace TestInputOutput
{
	std::ifstream *prepareInput(const std::initializer_list<std::string> &list)
	{
		const std::string fileName = "testFile.txt";

		std::ofstream out(fileName);
		for (std::string line : list)
			out << line << '\n';
		out.close();

		return new std::ifstream(fileName);
	}

	TEST(InputStreamReader, TestIntegralReaderBaseFunctions)
	{
		std::ifstream *in = prepareInput({"    1   2 3 4 5 6 7 ", "7 6 5 4 trash 888notnumber numbernot888 3 2 1"});
		std::vector< std::vector<int> > answer = {{1, 2, 3, 4, 5, 6, 7}, {7, 6, 5, 4, 3, 2, 1}};
		InputStreamReader<int> reader(*in);
		int currentValue, lineNumber = 0;
		for (auto it = answer.begin(); it != answer.end(); ++it, ++lineNumber)
		{
			int tokenNumber = 0;
			for (auto jt = it->begin(); jt != it->end(); ++jt, ++tokenNumber)
			{
				ASSERT_TRUE(reader(currentValue)) << "Failed to read token #" << tokenNumber << " in line #" << lineNumber;
				EXPECT_EQ(currentValue, *jt) << "Failed line #" << lineNumber << " on token #" << tokenNumber;
			}
		}
		ASSERT_FALSE(reader(currentValue));
		in->close();
		delete in;
	}
};

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
