#include <iostream>

#include <gtest/gtest.h>
#include "model/generalsegmenttree.h"

using namespace std;

int main(int argc, char **argv)
{
	GeneralSegmentTree<int, int, int, int, int> tree(0, 0);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

