#include <gtest/gtest.h>
#include "Tests.h"
#include <algorithm>
int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc,argv);
	RUN_ALL_TESTS();
	std::getchar();
}
