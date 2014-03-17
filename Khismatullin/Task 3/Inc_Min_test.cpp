#include "gtest/gtest.h"
#include "Inc_Min_Stree.h"
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
using namespace std;

#define pb push_back

bool operator ==(const IMn_Type &a, const IMn_Type &b)
{
	return (a.get() == b.get());
}

TEST(Unit_test, constructor)
{
	vector<int> a;
	for(int i = 0; i < 10; i++)
	{
		a.pb(rand()%10);
	}
	Stree<IMn_Type, IM
}

TEST(Unit_test, build)
{
	vector<int> a;
	for(int i = 0; i < 10; i++)
	{
		a.pb(rand()%10);
	}
	Stree<IMn_Type, IM
}

TEST(Manual_test, build)
{
	vector<IMn_Type> a;
	a.pb(IMn_Type(1));
	a.pb(IMn_Type(2));
	a.pb(IMn_Type(3));
	Stree<IMn_Type, IMn_Upd> tree(a);
}

TEST(Manual_test, get)
{
	vector<IMn_Type> a;
	a.pb(IMn_Type(1));
	a.pb(IMn_Type(2));
	a.pb(IMn_Type(3));
	Stree<IMn_Type, IMn_Upd> tree(a);
	EXPECT_EQ(IMn_Type(1), tree.get(1, 1));
	EXPECT_EQ(IMn_Type(2), tree.get(2, 2));
	EXPECT_EQ(IMn_Type(3), tree.get(3, 3));
	EXPECT_EQ(IMn_Type(1), tree.get(1, 2));
	EXPECT_EQ(IMn_Type(1), tree.get(1, 3));
	EXPECT_EQ(IMn_Type(2), tree.get(2, 3));
}

TEST(Manual_test, update)
{
	vector<IMn_Type> a;
	a.pb(IMn_Type(1));
	a.pb(IMn_Type(5));
	a.pb(IMn_Type(3));
	a.pb(IMn_Type(4));
	a.pb(IMn_Type(2));
	Stree<IMn_Type, IMn_Upd> tree(a);
	tree.update(1, 5, IMn_Upd(1));
	EXPECT_EQ(IMn_Type(2), tree.get(1, 3));
	EXPECT_EQ(IMn_Type(4), tree.get(2, 4));
	EXPECT_EQ(IMn_Type(3), tree.get(3, 5));
	EXPECT_EQ(IMn_Type(2), tree.get(1, 4));
	EXPECT_EQ(IMn_Type(3), tree.get(2, 5));
	EXPECT_EQ(IMn_Type(2), tree.get(1, 5));
}

TEST(Manual_test, Small_test)
{
	vector<IMn_Type> a;
	for(int i = 0; i < 6; i++)
		a.pb(IMn_Type(0));
	Stree<IMn_Type, IMn_Upd> tree(a);
	
	tree.update(2, 3, IMn_Upd(1));
	tree.update(1, 4, IMn_Upd(-1));
	tree.update(1, 4, IMn_Upd(1));
	
	EXPECT_EQ(IMn_Type(0), tree.get(1, 4));
	EXPECT_EQ(IMn_Type(1), tree.get(2, 3));
	
	tree.update(2, 3, IMn_Upd(-1));
	tree.update(1, 3, IMn_Upd(2));
	tree.update(4, 6, IMn_Upd(3));
	tree.update(3, 4, IMn_Upd(-2));
	
	EXPECT_EQ(IMn_Type(0), tree.get(1, 6));
	EXPECT_EQ(IMn_Type(2), tree.get(1, 2));
	EXPECT_EQ(IMn_Type(1), tree.get(4, 6));
	EXPECT_EQ(IMn_Type(0), tree.get(3, 4));
}



int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}