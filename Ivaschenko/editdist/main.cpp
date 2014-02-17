#include <iostream>

using namespace std;

#include <gtest/gtest.h>

#include "editdist.h"

int main(int argc, char **argv)
{
	std::string a = "somer", b = "omes";
	auto v = editDistance(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<char>());
	for (std::string s : v) cout << s << endl;

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

