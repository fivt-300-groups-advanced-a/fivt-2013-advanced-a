#include <gtest/gtest.h>
#include "Tests.h"
int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc,argv);
	RUN_ALL_TESTS();
	std::getchar();
}
