#ifndef TESTS
#define TESTS

#include "assigmentTree.h"
#include "increaseTree.h"
#include "ass_incTree.h"
#include "vectorTree.h"

TEST(FunctorTesing,SumMinMaxAssign_Increase_AssignIncreaseFunctors)
{

}

TEST(ManualTests,SumMinMaxAssignTree)
{
	int a[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	SumMinMaxAssignTree<int, 0, LONG_MAX, LONG_MIN> tree(a, a+10);
	// 0 1 2 3 4 5 6 7 8 9
	EXPECT_EQ(tree.get(1, 10).sum, 45);
	EXPECT_EQ(tree.get(2, 4).sum, 6);
	EXPECT_EQ(tree.get(3, 8).min, 2);
	EXPECT_EQ(tree.get(2, 10).max, 9);
	tree.assign(3,6,-1);
	// 0 1 -1 -1 -1 -1 6 7 8 9
	EXPECT_EQ(tree.get(1, 4).sum, -1);
	EXPECT_EQ(tree.get(1, 3).min, -1);
	EXPECT_EQ(tree.get(6, 8).max, 7);
	tree.assign(4,7,0);
	// 0 1 -1 0 0 0 0 7 8 9 
	EXPECT_EQ(tree.get(1, 10).sum, 24);
	EXPECT_EQ(tree.get(1, 10).min, -1);
	EXPECT_EQ(tree.get(1, 10).max, 9);
	tree.assign(1,9,-1);
	// -1 -1 -1 -1 -1 -1 -1 -1 9
	EXPECT_EQ(tree.get(1, 10).sum, 0);
	EXPECT_EQ(tree.get(2, 4).min, -1);
	EXPECT_EQ(tree.get(3, 6).max, -1);
	tree.assign(4,8,8);
	// -1 -1 -1 8 8 8 8 8 -1 9
	EXPECT_EQ(tree.get(2, 4).sum, 6);
	EXPECT_EQ(tree.get(8, 10).sum, 16);
	EXPECT_EQ(tree.get(3, 4).max, 8);
	EXPECT_EQ(tree.get(5, 5).min, 8);
}

TEST(ManualTests,SumMinMaxIncreaseTree)
{
	int a[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	SumMinMaxIncreaseTree<int, 0, LONG_MAX, LONG_MIN> tree(a, a + 10);
	// 0 0 0 0 0 0 0 0 0 0
	EXPECT_EQ(tree.get(1, 8).sum, 0);
	tree.increase(3,5,1);
	//0 0 1 1 1 0 0 0 0 0
	EXPECT_EQ(tree.get(2, 3).max, 1);
	EXPECT_EQ(tree.get(1, 4).sum, 2);
	tree.increase(1,9,2);
	//2 2 3 3 3 2 2 2 2 0
	tree.increase(2,3,1);
	//2 3 4 3 3 2 2 2 2 0
	EXPECT_EQ(tree.get(1, 4).max, 4);
	EXPECT_EQ(tree.get(3, 5).sum, 10);
	tree.increase(9, 10, 5);
	//2 3 4 3 3 2 2 2 7 5
	EXPECT_EQ(tree.get(8, 10).min, 2);
	EXPECT_EQ(tree.get(9, 9).max, 7);
	tree.increase(5,6,3);
	//2 3 4 3 6 5 2 2 7 5
	EXPECT_EQ(tree.get(1, 8).max, 6);
	EXPECT_EQ(tree.get(1, 10).sum, 39);
}

TEST(ManualTests,SumMinMaxAssignIncreaseTree)
{
	int a[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	SumMinMaxIncreaseAssignTree<int, 0, LONG_MAX, LONG_MIN> tree(a, a + 10);
	// 0 0 0 0 0 0 0 0 0 0 
	tree.assign(1, 2, 1);
	// 1 1 0 0 0 0 0 0 0 0
	EXPECT_EQ(tree.get(1, 3).sum, 2);
	tree.increase(2, 4, 2);
	// 1 3 2 2 0 0 0 0 0 0
	EXPECT_EQ(tree.get(1, 10).sum, 8);
	tree.assign(8, 10, 3);
	// 1 3 2 2 0 0 0 3 3 3
	EXPECT_EQ(tree.get(3, 10).sum, 13);
	tree.increase(1, 8, 1);
	// 2 4 3 3 1 1 1 4 3 3
	EXPECT_EQ(tree.get(6,10).max, 4);
	EXPECT_EQ(tree.get(1,10).sum, 25);
	tree.assign(4, 5, 5);
	// 2 4 3 5 5 1 1 4 3 3
	EXPECT_EQ(tree.get(5, 6).max, 5);
	EXPECT_EQ(tree.get(3, 6).sum, 14);
	tree.increase(1, 10, 1);
	tree.increase(1, 10, 1);
	tree.increase(1, 10, 1);
	// 5 7 6 8 8 4 4 7 6 6
	EXPECT_EQ(tree.get(1, 4).sum, 26);
	tree.increase(1, 10, -5);
	// 0 2 1 3 3 -1 -1 2 1 1
	EXPECT_EQ(tree.get(6, 10).min, -1);
	tree.assign(1, 10, 0);
	// 0 0 0 0 0 0 0 0 0 0
	EXPECT_EQ(tree.get(2, 2).sum, 0);
	EXPECT_EQ(tree.get(1, 10).max, 0);
}

const int number_of_operations = 100000;
const int size_of_data = 1000;
const int max_random_number = 100000;

void get_random_borders(int& left, int& right)
{
	right = rand() % size_of_data + 1;
	left = rand() % right + 1;
}

TEST(StressTest,SumMinMaxAssignIncreaseTree_vs_Vector)
{
	std::vector<int> data;
	for (unsigned int i=0; i < size_of_data; i++)
		data.push_back(rand() % max_random_number);

	vectorTree<int> slow_tree (data.begin(), data.end());
	SumMinMaxIncreaseAssignTree<int, 0, LONG_MAX, LONG_MIN> fast_tree (data.begin(), data.end());

	int left, right;
	for (int i=0; i < number_of_operations; i++)
	{
		int operation = rand() % 5;
		int change = rand() % max_random_number;
		get_random_borders(left, right);
		if (operation==0) // increasing
		{
			fast_tree.increase(left, right, change);
			slow_tree.increase(left, right, change);
		} 
		else if (operation==1) // assigning
		{
			fast_tree.assign(left, right, change);
			slow_tree.assign(left, right, change);
		}
		else if (operation==2) // get Sum
		{
			EXPECT_EQ( fast_tree.get(left, right).sum, slow_tree.getSum(left, right) );
		} 
		else if (operation==3) // get Min
		{
			EXPECT_EQ( fast_tree.get(left, right).min, slow_tree.getMin(left, right) );
		} 
		else // get Max
		{
			EXPECT_EQ( fast_tree.get(left, right).max, slow_tree.getMax(left, right) );
		}
	}
}

#endif TESTS