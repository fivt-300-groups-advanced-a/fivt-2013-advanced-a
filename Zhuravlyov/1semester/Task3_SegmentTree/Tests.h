#ifndef TESTS
#define TESTS

#include "assigmentTree.h"
#include "increaseTree.h"
#include "ass_incTree.h"
#include "NumbOfPermanenceSegmentsTree.h"
#include "maxSubsegmentTree.h"
#include "vectorTree.h"
#include "fakeTree.h"
#include "friendly_structures.h"
#include "File_IO.h"


TEST(InnerStateTesing, TestAdvancedTree)
{
	std::vector<int> a;
	for (int i=1024; i<= 1024 + 513; i++)
		a.push_back(i);
	{
		// tree with 514 elements
		fakeTree tree(a.begin(), a.end());
		EXPECT_TRUE(correct_build);
		// in building process we made 1023 unions to fill other verteces
		EXPECT_EQ(number_of_unions, 1023);
		number_of_unions = 0;

		tree.change(3, 512); 
		// Meta information adds and pushes to segments:
		// [3,4], [5,8], [9,16], [17,32], [33,64], [65,128], [129,256], [257,512]
		// +8 metaQuantity, and +8 pushes summary
		EXPECT_EQ(number_of_pushes, 8);
		EXPECT_EQ(number_of_merges, 0);
		EXPECT_EQ(metaQuantity, 8);
		EXPECT_TRUE(number_of_unions <= 20);
		number_of_pushes = number_of_unions = 0;

		tree.change(8, 13);
		// Need to change information on segments:
		// [8, 8], [9, 12], [13,13]
		// [5, 8] -> shove to [5,6], [7,8] +1 metaQuantity, +2 pushes
		// [7, 8] -> shove to [7,7], [8,8] +1 metaQuantity, +2 pushes, 
		// -2 meta, cause [7,7] and [8,8] are leaves
		// [8,8] - merge information with additional +1 push, no merges, it's a leaf
		// [9, 16] -> shove to [9, 12], [13, 16] +1 metaQuantity, +2 pushes
		// [9,12] - merge information with additional +1 merge, +1 push
		// [13, 16] -> shove to [13,14], [15,16] +1 metaQuantity, +2 pushes
		// [13, 14] -> shove to [13,13], [14,14] +1 metaQuantity, +2 pushes
		// -2 meta [13,13], [14,14] are a leaves
		// [13, 13] - merge information with additional +1 push, no merges
		// +1 metaQuantity = 9, +13 pushes, +1 merge summary
		EXPECT_EQ(metaQuantity, 9);
		EXPECT_EQ(number_of_pushes, 13);
		EXPECT_EQ(number_of_merges, 1);
		EXPECT_TRUE(number_of_unions <= 20);
		number_of_pushes = number_of_merges = number_of_unions = 0;

		tree.get(2,3);
		// get information at [2,2], [3,3] 
		// [3,4] -> shove to [3,3], [4,4] +1 meta, +2 pushes
		// -2 meta because of leaves
		EXPECT_EQ(metaQuantity, 8);
		EXPECT_EQ(number_of_pushes, 2);
		EXPECT_EQ(number_of_merges, 0);
		EXPECT_TRUE(number_of_unions <= 20);
		number_of_pushes = number_of_merges = number_of_unions = 0;

		tree.change(1, 514);
		// Meta adds to [1, 512], [513,514], +2 meta, +2 pushes
		tree.change(1, 512);
		tree.change(1, 512);
		// Merge meta with [1,512] twice, +2 push, +2 merge
		EXPECT_EQ(metaQuantity, 10);
		EXPECT_EQ(number_of_pushes, 4);
		EXPECT_EQ(number_of_merges, 2);
		number_of_pushes = number_of_merges = number_of_unions = 0;

		tree.get(1, 256);
		// shove [1,512] -> [1,256], [257,512] - +1 meta, +2 pushes
		// [257, 512] - +1 merge with old information, so -1 meta
		EXPECT_EQ(metaQuantity, 10);
		EXPECT_EQ(number_of_pushes, 2);
		EXPECT_EQ(number_of_merges, 1);
		EXPECT_TRUE(number_of_unions <= 20);
		number_of_pushes = number_of_merges = number_of_unions = 0;
	}
	EXPECT_EQ(metaQuantity, 0);
	EXPECT_TRUE(correct_push);
	EXPECT_TRUE(correct_merge);
	EXPECT_TRUE(correct_union);
}

TEST(FunctorTesting, SumMinMaxAssignTreeFunctors)
{
	typedef SumMinMaxAssignTreeTestFriend::ReturnType ReturnType;
	typedef SumMinMaxAssignTreeTestFriend::MetaInformation MetaInformation;
	SumMinMaxAssignTreeTestFriend::Unite unifier;
	SumMinMaxAssignTreeTestFriend::Merge merger;
	SumMinMaxAssignTreeTestFriend::Push pusher;
	ReturnType first(10, 20, 30);
	ReturnType second(-5);
	// Test unifier with neutral
	first = unifier(first, ReturnType());
	EXPECT_EQ(first.sum, 10);
	EXPECT_EQ(first.min, 20);
	EXPECT_EQ(first.max, 30);
	second = unifier(ReturnType(), second);
	EXPECT_EQ(second.sum, -5);
	EXPECT_EQ(second.min, -5);
	EXPECT_EQ(second.max, -5);
	// Test unifier
	ReturnType third = unifier(first, second);
	EXPECT_EQ(third.sum, 5);
	EXPECT_EQ(third.min, -5);
	EXPECT_EQ(third.max, 30);
	// Test merge
	auto metaFirstPointer = new MetaInformation(3);
	auto metaSecondPointer = new MetaInformation(5);
	merger(metaFirstPointer, metaSecondPointer);
	EXPECT_EQ( metaFirstPointer->assigned, 5);
	merger(metaSecondPointer, &MetaInformation(4) );
	EXPECT_EQ( metaSecondPointer->assigned, 4);
	// Test push
	pusher(first, metaFirstPointer, 3);
	EXPECT_EQ(first.sum, 15);
	EXPECT_EQ(first.min, 5);
	EXPECT_EQ(first.max, 5);
}

TEST(FunctorTesting, SumMinMaxIncreaseTreeFunctors)
{
	typedef SumMinMaxIncreaseTreeTestFriend::ReturnType ReturnType;
	typedef SumMinMaxIncreaseTreeTestFriend::MetaInformation MetaInformation;
	SumMinMaxIncreaseTreeTestFriend::Merge merger;
	SumMinMaxIncreaseTreeTestFriend::Push pusher;
	// unifier is already tested in previous tree, it's the same here
	// test merger
	auto metaFirst = new MetaInformation(3);
	auto metaSecond = new MetaInformation(-6);
	merger(metaFirst, metaSecond);
	EXPECT_EQ(metaFirst->increased, -3);
	// test pusher
	ReturnType first(3, 0, -6);
	pusher(first, metaFirst, 1000);
	EXPECT_EQ(first.sum, 3 - 1000 * 3);
	EXPECT_EQ(first.min, -3);
	EXPECT_EQ(first.max, -9);
}

TEST(FunctorTesting, SumMinMaxIncreaseAssignTreeFunctors)
{
	typedef SumMinMaxIncreaseAssignTreeTestFriend::ReturnType ReturnType;
	typedef SumMinMaxIncreaseAssignTreeTestFriend::MetaInformation MetaInformation;
	SumMinMaxIncreaseAssignTreeTestFriend::Merge merger;
	SumMinMaxIncreaseAssignTreeTestFriend::Push pusher;
	// unifier is still the same
	// test merger
	auto MetaFirst = new MetaInformation(false, 3); // assign 3
	auto MetaSecond = new MetaInformation(true, 5); // increase 5
	auto MetaThird = new MetaInformation(false, 6); // assign 6
	auto MetaFourth = new MetaInformation(true, -3); // decrease 3
	merger(MetaFirst, MetaThird); // merge 1 and 3
	EXPECT_EQ(MetaFirst->operation, false);
	EXPECT_EQ(MetaFirst->changed, 6); // got assign 6
	merger(MetaSecond, MetaFourth); // merge 2 and 4
	EXPECT_EQ(MetaSecond->operation, true);
	EXPECT_EQ(MetaSecond->changed, 2); // got increase 2
	merger(MetaFirst, MetaSecond); // merge increase 2 to assign 6
	EXPECT_EQ(MetaFirst->operation, false);
	EXPECT_EQ(MetaFirst->changed, 8); // got assign 8
	merger(MetaSecond, MetaFirst); // merge assign 8 to increase 2
	EXPECT_EQ(MetaSecond->operation, false);
	EXPECT_EQ(MetaSecond->changed, 8); // got assign 8
	// test pusher
	ReturnType first(5, 6, 7);
	pusher(first, MetaFourth, 5); // decrease 3
	EXPECT_EQ(first.sum, -10);
	EXPECT_EQ(first.min, 3);
	EXPECT_EQ(first.max, 4);
	pusher(first, MetaFirst, 2); // assign 8
	EXPECT_EQ(first.sum, 16);
	EXPECT_EQ(first.min, 8);
	EXPECT_EQ(first.max, 8);
}

TEST(FunctorTesting, NumberOfPermanenceSegmentsFunctors)
{
	typedef NumberOfPermanenceSegmentsTreeTestFriend::ReturnType ReturnType;
	typedef NumberOfPermanenceSegmentsTreeTestFriend::MetaInformation MetaInformation;
	NumberOfPermanenceSegmentsTreeTestFriend::Unite unifier;
	NumberOfPermanenceSegmentsTreeTestFriend::Push pusher;
	// test unifier
	ReturnType first(1, 3, 6);
	ReturnType second(3, 1, 2);
	first = unifier(first, ReturnType());
	EXPECT_EQ(first.left, 1);
	EXPECT_EQ(first.right, 3);
	EXPECT_EQ(first.number, 6);
	second = unifier(ReturnType(), second);
	EXPECT_EQ(second.left, 3);
	EXPECT_EQ(second.right, 1);
	EXPECT_EQ(second.number, 2);
	ReturnType third = unifier(first, second);
	EXPECT_EQ(third.left, 1);
	EXPECT_EQ(third.right, 1);
	EXPECT_EQ(third.number, 7);
	third = unifier(first, third);
	EXPECT_EQ(third.left, 1);
	EXPECT_EQ(third.right, 1);
	EXPECT_EQ(third.number, 13);
	//merge is same as SumMinMaxIncreaseAssignTree, no need to test it
	// test push
	auto metaFirst = new MetaInformation(false, 2); // assign 2
	auto metaSecond = new MetaInformation(true, 3); // increase 3
	pusher(third, metaFirst, 10);
	EXPECT_EQ(third.left, 2);
	EXPECT_EQ(third.right, 2);
	EXPECT_EQ(third.number, 1);
	pusher(first, metaSecond, 10);
	EXPECT_EQ(first.left, 4);
	EXPECT_EQ(first.right, 6);
	EXPECT_EQ(first.number, 6);
}

TEST(FunctorTesting, MaxSubsegmentFunctors)
{
	typedef MaxSubsegmentTreeTestFriend::ReturnType ReturnType;
	typedef MaxSubsegmentTreeTestFriend::MetaInformation MetaInformation;
	MaxSubsegmentTreeTestFriend::Unite unifier;
	MaxSubsegmentTreeTestFriend::Push pusher;
	// test unifier
	ReturnType first(3, 2, 4, -8); 
	// max_prefix, max_suffix, max_subsum, sum
	ReturnType second(3, 4, 2, -9);
	first = unifier(first, ReturnType());
	second = unifier(ReturnType(), second);
	EXPECT_EQ(first.max_prefix, 3);
	EXPECT_EQ(first.max_suffix, 2);
	EXPECT_EQ(first.max_subsum, 4);
	EXPECT_EQ(first.sum, -8);
	EXPECT_EQ(second.max_prefix, 3);
	EXPECT_EQ(second.max_suffix, 4);
	EXPECT_EQ(second.max_subsum, 2);
	EXPECT_EQ(second.sum, -9);
	ReturnType third = unifier(first, second);
	EXPECT_EQ(third.max_prefix, 3);
	EXPECT_EQ(third.max_suffix, 4);
	EXPECT_EQ(third.max_subsum, 5);
	EXPECT_EQ(third.sum, -17);
	ReturnType fourth(-1, -1, -1, -8);
	ReturnType fifth(8 , -1 , 9, -3);
	ReturnType sixth = unifier(fourth, fifth);
	EXPECT_EQ(sixth.max_prefix, 0);
	EXPECT_EQ(sixth.max_suffix, -1);
	EXPECT_EQ(sixth.max_subsum, 9);
	EXPECT_EQ(sixth.sum, -11);
	ReturnType seventh(1, 4, 9, -12);
	ReturnType eighth(7, -5, 5, -5);
	ReturnType ninth = unifier(seventh, eighth);
	EXPECT_EQ(ninth.max_prefix, 1);
	EXPECT_EQ(ninth.max_suffix, -1);
	EXPECT_EQ(ninth.max_subsum, 11);
	EXPECT_EQ(ninth.sum, -17);
	// merge is already tested in SumMinMaxAssignTree
	// test push
	auto metaFirst = new MetaInformation(5);
	pusher(ninth, metaFirst, 10);
	EXPECT_EQ(ninth.max_prefix, 50);
	EXPECT_EQ(ninth.max_suffix, 50);
	EXPECT_EQ(ninth.max_subsum, 50);
	EXPECT_EQ(ninth.sum, 50);
	auto metaSecond = new MetaInformation(-1);
	pusher(first, metaSecond, 5);
	EXPECT_EQ(first.max_prefix, -1);
	EXPECT_EQ(first.max_suffix, -1);
	EXPECT_EQ(first.max_subsum, -1);
	EXPECT_EQ(first.sum, -5);
}

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
	MaxSubsegmentTree<int, 0, LONG_MIN> tree(a, a+10);
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

enum tree_operations { increase, assign, sum, min, max, maxSubSum, PermSegm };

void get_random_borders(int& left, int& right, int max_size)
{
	right = rand() % max_size + 1;
	left = rand() % right + 1;
}

void generate_input_data(const std::string& filename, int data_size, int operations_number,
							int max_value, const std::vector<int>& operations)
{
	BinaryFileWriter write(filename);
	write(data_size);
	for (int i=0; i<data_size; i++)
		write(rand() % max_value);
	write(operations_number);
	for (int i = 0; i < operations_number; i++)
	{
		int left, right;
		get_random_borders(left, right, data_size);
		int operation = operations[rand() % operations.size()]; // random operation
		write( operation ); 
		write(left); write(right); // random segment
		if (operation == increase || operation == assign) 
			write(rand()%1000); // additional data if it's modify operation
	}
	write.close();
}

void generate_output_data(const std::string& input_filename, const std::string& output_filename)
{
	BinaryFileReader read(input_filename);
	BinaryFileWriter write(output_filename);
	int number_of_elements;
	read(number_of_elements);
	std::vector<int> start_data(number_of_elements);
	for (int i = 0; i < number_of_elements; i++)
		read(start_data[i]);
	vectorTree<int, 0> slow_tree(start_data.begin(), start_data.end());
	int number_of_operations;
	read(number_of_operations);
	for (int i = 0; i< number_of_operations; i++)
	{
		int operation, left, right, change;
		read(operation);
		read(left); read(right);
		if (operation == increase)
		{
			read(change);
			slow_tree.increase(left, right, change);
		} 
		else if (operation == assign)
		{
			read(change);
			slow_tree.assign(left, right, change);
		}
		else if (operation == sum) 
		{
			write(slow_tree.getSum(left, right));
		} 
		else if (operation == min)
		{
			write(slow_tree.getMin(left, right));
		}
		else if (operation == max) 
		{
			write(slow_tree.getMax(left, right));
		}
		else if (operation == maxSubSum)
		{
			write(slow_tree.maxSubSum(left, right));
		}
		else if (operation == PermSegm) // number of permanence segments
		{
			write(slow_tree.NumberOfPermSegments(left, right));
		}
	}
	read.close();
	write.close();
}

void initializate_files(const std::string& input_filename, const std::string& output_filename,
						const std::vector<int>& operations)
{
	const int tests_size_of_data = 1000000;
	const int tests_operations_number = 100000;
	const int max_value = 2000;
	if (!FileIsExist(input_filename)) 
	{
		generate_input_data(input_filename, tests_size_of_data, 
							tests_operations_number, max_value, operations);
	}
	if (!FileIsExist(output_filename)) 
	{
		generate_output_data(input_filename, output_filename);
	}
}

TEST(IntergationTests, SumMinMaxAssignTree)
{
	std::vector<int> operations(4);
	operations[0] = sum; operations[1] = min;
	operations[2] = max; operations[3] = assign;
	initializate_files("SumMinMaxAssignTree.dat", "SumMinMaxAssignTree.sol", operations);
	BinaryFileReader input("SumMinMaxAssignTree.dat");
	BinaryFileReader output("SumMinMaxAssignTree.sol");
	int number_of_elements;
	input(number_of_elements);
	std::vector<int> data(number_of_elements);
	for (int i = 0; i < number_of_elements; i++)
		input(data[i]);
	SumMinMaxAssignTree<int, 0, LONG_MAX, LONG_MIN> tree(data.begin(), data.end());

	int number_of_operations;
	input(number_of_operations);
	for (int i = 0; i < number_of_operations; i++)
	{
		int operation_type, left, right, change;
		input(operation_type);
		input(left); input(right);
		if (operation_type == assign) 
		{
			input(change);
			tree.assign(left, right, change);
		}
		else
		{
			int expected;
			output(expected);
			if (operation_type == sum)
				EXPECT_EQ(tree.get(left, right).sum, expected);
			else if (operation_type == min)
				EXPECT_EQ(tree.get(left, right).min, expected);
			else if (operation_type == max)
				EXPECT_EQ(tree.get(left, right).max, expected);
		}
	}
	input.close();
	output.close();
}

TEST(IntergationTests, SumMinMaxIncreaseTree)
{
	std::vector<int> operations(4);
	operations[0] = sum; operations[1] = min;
	operations[2] = max; operations[3] = increase;
	initializate_files("SumMinMaxIncreaseTree.dat", "SumMinMaxIncreaseTree.sol", operations);
	BinaryFileReader input("SumMinMaxIncreaseTree.dat");
	BinaryFileReader output("SumMinMaxIncreaseTree.sol");
	int number_of_elements;
	input(number_of_elements);
	std::vector<int> data(number_of_elements);
	for (int i = 0; i < number_of_elements; i++)
		input(data[i]);
	SumMinMaxIncreaseTree<int, 0, LONG_MAX, LONG_MIN> tree(data.begin(), data.end());

	int number_of_operations;
	input(number_of_operations);
	for (int i = 0; i < number_of_operations; i++)
	{
		int operation_type, left, right, change;
		input(operation_type);
		input(left); input(right);
		if (operation_type == increase) 
		{
			input(change);
			tree.increase(left, right, change);
		}
		else
		{
			int expected;
			output(expected);
			if (operation_type == sum)
				EXPECT_EQ(tree.get(left, right).sum, expected);
			else if (operation_type == min)
				EXPECT_EQ(tree.get(left, right).min, expected);
			else if (operation_type == max)
				EXPECT_EQ(tree.get(left, right).max, expected);
		}
	}
	input.close();
	output.close();
}

TEST(IntergationTests, SumMinMaxIncreaseAssignTree)
{
	std::vector<int> operations(4);
	operations[0] = sum; operations[1] = min;
	operations[2] = max; operations[3] = increase;
	initializate_files("SumMinMaxIncreaseAssignTree.dat", "SumMinMaxIncreaseAssignTree.sol", operations);
	BinaryFileReader input("SumMinMaxIncreaseAssignTree.dat");
	BinaryFileReader output("SumMinMaxIncreaseAssignTree.sol");
	int number_of_elements;
	input(number_of_elements);
	std::vector<int> data(number_of_elements);
	for (int i = 0; i < number_of_elements; i++)
		input(data[i]);
	SumMinMaxIncreaseAssignTree<int, 0, LONG_MAX, LONG_MIN> tree(data.begin(), data.end());

	int number_of_operations;
	input(number_of_operations);
	for (int i = 0; i < number_of_operations; i++)
	{
		int operation_type, left, right, change;
		input(operation_type);
		input(left); input(right);
		if (operation_type == increase) 
		{
			input(change);
			tree.increase(left, right, change);
		}
		else if (operation_type == assign)
		{
			input(change);
			tree.assign(left, right, change);
		}
		else
		{
			int expected;
			output(expected);
			if (operation_type == sum)
				EXPECT_EQ(tree.get(left, right).sum, expected);
			else if (operation_type == min)
				EXPECT_EQ(tree.get(left, right).min, expected);
			else if (operation_type == max)
				EXPECT_EQ(tree.get(left, right).max, expected);
		}
	}
	input.close();
	output.close();
}

TEST(IntergationTests, NumberOfPermanenceSegmentsTree)
{
	std::vector<int> operations(3);
	operations[0] = increase; operations[1] = assign;
	operations[2] = PermSegm;
	initializate_files("NumberOfPermanenceSegmentsTree.dat", "NumberOfPermanenceSegmentsTree.sol", operations);
	BinaryFileReader input("NumberOfPermanenceSegmentsTree.dat");
	BinaryFileReader output("NumberOfPermanenceSegmentsTree.sol");
	int number_of_elements;
	input(number_of_elements);
	std::vector<int> data(number_of_elements);
	for (int i = 0; i < number_of_elements; i++)
		input(data[i]);
	NumberOfPermanenceSegmentsTree<int> tree(data.begin(), data.end());

	int number_of_operations;
	input(number_of_operations);
	for (int i = 0; i < number_of_operations; i++)
	{
		int operation_type, left, right, change;
		input(operation_type);
		input(left); input(right);
		if (operation_type == increase) 
		{
			input(change);
			tree.increase(left, right, change);
		}
		else if (operation_type == assign)
		{
			input(change);
			tree.assign(left, right, change);
		}
		else
		{
			int expected;
			output(expected);
			EXPECT_EQ(tree.getNumber(left, right), expected);
		}
	}
	input.close();
	output.close();
}

TEST(IntergationTests, MaxSubsegmentTree)
{
	std::vector<int> operations(2);
	operations[0] = assign;
	operations[1] = maxSubSum;
	initializate_files("MaxSubsegmentTree.dat", "MaxSubsegmentTree.sol", operations);
	BinaryFileReader input("MaxSubsegmentTree.dat");
	BinaryFileReader output("MaxSubsegmentTree.sol");
	int number_of_elements;
	input(number_of_elements);
	std::vector<int> data(number_of_elements);
	for (int i = 0; i < number_of_elements; i++)
		input(data[i]);
	MaxSubsegmentTree<int, 0, LONG_MIN> tree(data.begin(), data.end());

	int number_of_operations;
	input(number_of_operations);
	for (int i = 0; i < number_of_operations; i++)
	{
		int operation_type, left, right, change;
		input(operation_type);
		input(left); input(right);
		if (operation_type == assign)
		{
			input(change);
			tree.assign(left, right, change);
		}
		else
		{
			int expected;
			output(expected);
			EXPECT_EQ(tree.getMaxSubSum(left, right), expected);
		}
	}
	input.close();
	output.close();
}

const int number_of_operations = 100000;
const int size_of_data = 100000;
const int max_random_number = 20000;

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
		get_random_borders(left, right, size_of_data);
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