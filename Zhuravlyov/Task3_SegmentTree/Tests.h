#ifndef TESTS
#define TESTS

#include "assigmentTree.h"
#include "increaseTree.h"
#include "ass_incTree.h"
#include "NumbOfPermanenceSegmentsTree.h"
#include "maxSubsegmentTree.h"
#include "vectorTree.h"
//#include "fakeTree.h"

/*
TEST(InnerStateTesing, TestAdvancedTree)
{
	//to do
	std::vector<int> a;
	for (int i=512; i<= 512 + 513; i++)
		a.push_back(i);
	fakeTree tree(a.begin(), a.end());
	EXPECT_TRUE(tree.correct_build);
}

TEST(FunctorTesing, SumMinMaxAssign_Increase_AssignIncreaseFunctors)
{
	//todo
}
*/

TEST(ManualTests, SumMinMaxAssignTree)
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

TEST(ManualTests, SumMinMaxIncreaseTree)
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

TEST(ManualTests, SumMinMaxAssignIncreaseTree)
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

TEST(ManualTests, NumberOfPermanenceSegmentsTree)
{
	int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 8, 8 };
	NumberOfPermanenceSegmentsTree<int> tree(a, a+10);
	// 1 2 3 4 5 6 7 8 8 8
	EXPECT_EQ(tree.getNumber(1, 10), 8);
	tree.assign(2, 4, 0);
	// 1 0 0 0 5 6 7 8 8 8
	EXPECT_EQ(tree.getNumber(1, 3), 2);
	tree.increase(2, 7, 1);
	// 1 1 1 1 6 7 8 8 8 8
	EXPECT_EQ(tree.getNumber(1, 10), 4);
	tree.assign(6, 7, 1);
	// 1 1 1 1 6 1 1 8 8 8
	EXPECT_EQ(tree.getNumber(1, 7), 3);
	tree.increase(5, 5, -5);
	// 1 1 1 1 1 1 1 8 8 8
	EXPECT_EQ(tree.getNumber(1, 9), 2);
	tree.assign(3, 4, 2);
	// 1 1 2 2 1 1 1 8 8 8
	EXPECT_EQ(tree.getNumber(1, 10), 4);
	tree.increase(1, 9, 1);
	// 2 2 3 3 2 2 2 9 9 8
	EXPECT_EQ(tree.getNumber(1, 10), 5);
	tree.assign(1, 10, 0);
	// 0 0 0 0 0 0 0 0 0 0
	EXPECT_EQ(tree.getNumber(3, 7), 1);
}

TEST(ManualTests, MaxSubsegmentTree)
{
	int a[10] = {1, 2, -1, -1, 3, -2, -1, -5, 1, 2 };
	maxSubsegmentTree<int, 0, LONG_MIN> tree(a, a+10);
	// 1 2 -1 -1 3 -2 -1 -5 1 2
	EXPECT_EQ(tree.getMaxSubSum(1, 10), 4);
	tree.assign(4, 6, -2);
	// 1 2 -1 -2 -2 -2 -1 -5 1 2
	EXPECT_EQ(tree.getMaxSubSum(3, 4), -1);
	EXPECT_EQ(tree.getMaxSubSum(7, 10), 3);
	tree.assign(1, 4, 2);
	// 2 2 2 2 -2 -2 -1 -5 1 2
	EXPECT_EQ(tree.getMaxSubSum(1, 10), 8);
	tree.assign(8, 10, 1);
	tree.assign(10, 10, 5);
	// 2 2 2 2 -2 -2 -1 1 1 5
	EXPECT_EQ(tree.getMaxSubSum(1, 10), 10);
	tree.assign(2, 3, -10);
	// 2 -10 -10 2 -2 -2 -1 1 1 5
	EXPECT_EQ(tree.getMaxSubSum(2, 3), -10);
	EXPECT_EQ(tree.getMaxSubSum(6, 9), 2);
	tree.assign(1, 10, -1);
	//-1 -1 -1 -1 -1 -1 -1 -1 -1 -1
	EXPECT_EQ(tree.getMaxSubSum(2, 8), -1);
	tree.assign(2, 3, 2);
	// -1 2 2 -1 -1 -1 -1 -1 -1 -1
	EXPECT_EQ(tree.getMaxSubSum(1, 8), 4);
	tree.assign(9, 10, 1);
	//-1 2 2 -1 -1 -1 -1 -1 1 1
	EXPECT_EQ(tree.getMaxSubSum(1, 10), 4);
	EXPECT_EQ(tree.getMaxSubSum(5, 10), 2);
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

	vectorTree<int, 0> slow_tree (data.begin(), data.end());
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