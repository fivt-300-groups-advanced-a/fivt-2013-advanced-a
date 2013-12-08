#ifndef TESTS
#define TESTS

#include "specificTrees.h"

TEST(ManualTest,SumAssignTree)
{
	int a[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	SegmentTree_SumAssign_Int tree(a, a + 10);
	// 0 1 2 3 4 5 6 7 8 9
	EXPECT_EQ(tree.get(1, 10).sum, 45);
	EXPECT_EQ(tree.get(2,4).sum, 6);
	tree.change(3,6,-1);
	// 0 1 -1 -1 -1 -1 6 7 8 9
	EXPECT_EQ(tree.get(1,4).sum,-1);
	tree.change(4,7,0);
	// 0 1 -1 0 0 0 0 7 8 9 
	EXPECT_EQ(tree.get(1,10).sum,24);
	tree.change(1,9,-1);
	// -1 -1 -1 -1 -1 -1 -1 -1 9
	EXPECT_EQ(tree.get(1,10).sum,0);
	tree.change(4,8,8);
	// -1 -1 -1 8 8 8 8 8 -1 9
	EXPECT_EQ(tree.get(2,4).sum,6);
	EXPECT_EQ(tree.get(8,10).sum,16);
}

#endif TESTS