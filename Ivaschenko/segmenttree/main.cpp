#include <functional>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>
#include "model/generalsegmenttree.h"

using namespace std;

struct MyInfo
{

};

struct InfoUpdater
{
	public:
		int operator () (const int &a, const MyInfo &b)
		{
			return a;
		}
};

struct InfoMerger
{
	public:
		MyInfo operator () (const MyInfo &a, const MyInfo &b)
		{
			return MyInfo();
		}
};

int main(int argc, char **argv)
{
	GeneralSegmentTree<int, MyInfo, std::plus<int>, InfoUpdater, InfoMerger> tree(4, 0);
	assert(tree.get(0, 2) == 0);
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

