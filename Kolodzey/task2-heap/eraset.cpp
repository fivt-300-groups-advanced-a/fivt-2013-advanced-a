#include <set>
#include <vector>
#include <fstream>
#include <algorithm>
#include "gtest/gtest.h"
#include "biheap.h"
#include "access.h"

const int MAX_N_OF_INSERTIONS = 100500;
TEST(eraseInsertBySet, test1)
{
	std::ifstream in("eraset/01.in");
	std::multiset<int> myset;
	BiHeap<int> myheap;
	std::multiset<int>::iterator setIt[MAX_N_OF_INSERTIONS];
	ValPointer<int> heapIt[MAX_N_OF_INSERTIONS];
	int N;
	in >> N;
	int nInserted = 0;
	for (int i = 0; i < N; ++i)
	{
		int cmd;
		in >> cmd;
		if (cmd == 0)
		{
			int val;
			in >> val;
			setIt[nInserted] = myset.insert(val);
			heapIt[nInserted] = myheap.insert(val);
			++nInserted;
			EXPECT_EQ(myset.size(), myheap.size());
			EXPECT_EQ(*(myset.begin()), myheap.top());
		}
		else
		{
			int ind;
			in >> ind;
			myset.erase(setIt[ind]);
			myheap.erase(heapIt[ind]);
			EXPECT_EQ(myset.size(), myheap.size());
			EXPECT_EQ(*(myset.begin()), myheap.top());
		}
	}

	std::vector<int> vals;
	TestAccess<BiHeap<int>,int>::get_all_val(myheap,vals);
	sort(vals.begin(), vals.end());
	std::multiset<int>::iterator it;
	int i;
	for (it = myset.begin(), i = 0; it != myset.end(); ++it, ++i)
		EXPECT_EQ(vals[i], *it);
	in.close();
}