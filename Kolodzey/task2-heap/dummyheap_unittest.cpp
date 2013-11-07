#include "gtest/gtest.h"
#include "dummyheap.h"

TEST(DummyHeap, CreateInsertTop)
{
	DummyHeap <int> h;
	h.insert(1);
	EXPECT_EQ(h.top(), 1);
	h.insert(2);
	EXPECT_EQ(h.top(), 1);
	h.insert(0);
	EXPECT_EQ(h.top(), 0);
}

TEST(DummyHeap, UnusualCmp)
{
	DummyHeap <int, std::greater<int> > h((std::greater <int>()));
	h.insert(1);
	EXPECT_EQ(h.top(), 1);
	h.insert(2);
	EXPECT_EQ(h.top(), 2);
	h.insert(0);
	EXPECT_EQ(h.top(), 2);
}

TEST(DummyHeap, pop_top)
{
	DummyHeap <int> h;
	h.insert(1);
	h.insert(2);
	EXPECT_EQ(h.top(), 1);
	h.pop_top();
	EXPECT_EQ(h.top(), 2);
	h.insert(3);
	EXPECT_EQ(h.top(), 2);
	h.pop_top();
	EXPECT_EQ(h.top(), 3);
}

//incorrect test, checks throwing exeption
/*
TEST(DummyHeap, pop_topEXEPTION)
{
	DummyHeap <int> h;
	EXPECT_EQ(h.size(), 0);
	h.pop_top();
	EXPECT_EQ(h.size(), 0);

}
*/

TEST(DummyHeap, iterator)
{
	DummyHeap <int> h;
	h.insert(1);
	h.insert(2);
	h.insert(3);
	int j = 1;
	for (DummyHeap<int>::iterator i = h.begin(); i != h.end(); ++i)
	{
		EXPECT_EQ(*i, j);
		++j;
	}
	
	h.insert(6);
	h.insert(5);
	h.insert(4);
	j = 1;
	for (DummyHeap<int>::iterator i = h.begin(); i != h.end(); ++i)
	{
		EXPECT_EQ(*i, j);
		++j;
	}
}

TEST(DummyHeap, SizeEmptyClear)
{
	DummyHeap <int> h;
	EXPECT_EQ(h.size(), 0);
	EXPECT_EQ(h.empty(), 1);
	h.insert(1);
	EXPECT_EQ(h.size(), 1);
	EXPECT_EQ(h.empty(), 0);
	h.insert(2);
	EXPECT_EQ(h.size(), 2);
	EXPECT_EQ(h.empty(), 0);
	h.insert(3);
	EXPECT_EQ(h.size(), 3);
	EXPECT_EQ(h.empty(), 0);
	h.pop_top();
	EXPECT_EQ(h.size(), 2);
	EXPECT_EQ(h.empty(), 0);
	h.clear();
	EXPECT_EQ(h.size(), 0);
	EXPECT_EQ(h.empty(), 1);	
}

TEST(DummyHeap, absorb)
{
	DummyHeap <int> h1;
	h1.insert(1);
	h1.insert(2);
	h1.insert(6);
	DummyHeap <int> h2;
	h2.insert(3);
	h2.insert(5);
	h2.insert(4);
	h1.absorb(h2);
	EXPECT_EQ(h2.empty(), 1);
	int myints[6] = {1, 2, 3, 4, 5, 6};
	int j = 0;
	for (DummyHeap<int>::iterator i = h1.begin(); i != h1.end(); ++i)
	{
		EXPECT_EQ(*i, myints[j]);
		++j;
	}
}

TEST(DummyHeap, mergeconstructor)
{
	DummyHeap <int> h1;
	h1.insert(1);
	h1.insert(2);
	h1.insert(6);
	DummyHeap <int> h2;
	h2.insert(3);
	h2.insert(5);
	h2.insert(4);
	DummyHeap <int> h(h1, h2);
	EXPECT_EQ(h1.empty(), 1);
	EXPECT_EQ(h2.empty(), 1);
	int myints[6] = {1, 2, 3, 4, 5, 6};
	int j = 0;
	for (DummyHeap<int>::iterator i = h.begin(); i != h.end(); ++i)
	{
		EXPECT_EQ(*i, myints[j]);
		++j;
	}
}

TEST(DummyHeap, mergeconstructorUnusualcmp)
{
	DummyHeap <int, std::greater<int> > h1((std::greater <int>()));
	h1.insert(1);
	h1.insert(2);
	h1.insert(6);
	DummyHeap <int, std::greater<int> > h2((std::greater <int>()));
	h2.insert(3);
	h2.insert(5);
	h2.insert(4);
	DummyHeap <int, std::greater<int> > h(h1, h2);
	EXPECT_EQ(h1.empty(), 1);
	EXPECT_EQ(h2.empty(), 1);

	int myints[6] = {6, 5, 4, 3, 2, 1};
	int j = 0;
	for (DummyHeap<int>::iterator i = h.begin(); i != h.end(); ++i)
	{
		EXPECT_EQ(*i, myints[j]);
		++j;
	}
	h.insert(8);
	EXPECT_EQ(h.top(), 8);
}

//not compiling test, cheching, whether top() is const
/*
TEST(DummyHeap, isTopConst)
{
	DummyHeap <int> h;
	h.insert(2);
	h.top() = 3;
}
*/

TEST(DummyHeap, erase)
{
	DummyHeap <int> h;
	h.insert(1);
	h.insert(2);
	h.insert(3);
	h.insert(4);
	DummyHeap <int>::iterator it = h.begin();
	++it;
	h.erase(it);
	EXPECT_EQ(h.size(), 3);
	int myints[3] = {1, 3, 4};
	int j = 0;
	for (it = h.begin(); it != h.end(); ++it)
	{
		EXPECT_EQ(*it, myints[j]);
		++j;
	}
}