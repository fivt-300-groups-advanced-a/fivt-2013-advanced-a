#include <functional>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>
#include "model/generalsegmenttree.h"
#include "model/segmentadditiontree.h"

using namespace std;

int main(int argc, char **argv)
{
	vector<int> v = {1, 3, 2, 4};
	SegmentAdditionTree<int> tree(v.begin(), v.end(), -100, 100, 0);
	assert(tree.get(1, 2).sum == 5);
	tree.update(1, 3, 1);
	assert(tree.get(0, 2).sum == 8);
	assert(tree.get(0, 2).min == 1);
	assert(tree.get(0, 2).max == 4);

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


