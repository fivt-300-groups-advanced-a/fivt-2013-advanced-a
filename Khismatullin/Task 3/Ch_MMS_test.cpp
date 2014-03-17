#define TESTER
#include "gtest/gtest.h"
#include "Ch_MMS_Stree.h"
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
using namespace std;

#define pb push_back

bool operator ==(const CMMS_Type &a, const CMMS_Type &b)
{
	return (a.get_min() == b.get_min() && a.get_max() == b.get_max() && a.get_sum() == b.get_sum());
}

bool operator ==(const MMS &a, const MMS &b)
{
	return (a.min == b.min && a.max == b.max && a.sum == b.sum);
}

TEST(Unit_test, update)
{
	vector<CMMS_Type> v;
	v.pb(CMMS_Type(0));
	Stree<CMMS_Type, CMMS_Upd> tree(v);
	for(int i = 1; i <= 10; i++)
		tree.update(1, 1, CMMS_Upd(i));
	EXPECT_EQ(CMMS_Type(10), tree.get(1, 1));
}


TEST(Unit_test, push)
{
	vector<CMMS_Type> v;
	v.pb(CMMS_Type(0));
	v.pb(CMMS_Type(0));
	Stree<CMMS_Type, CMMS_Upd> tree(v);
	for(int i = 1; i <= 10; i++)
		tree.update(1, 2, CMMS_Upd(i));
	EXPECT_EQ(CMMS_Type(10), tree.get(1, 1));
	EXPECT_EQ(CMMS_Type(10), tree.get(2, 2));
	EXPECT_EQ(CMMS_Type(10, 10, 20), tree.get(1, 2));
}


TEST(Manual_test, build)
{
	vector<CMMS_Type> a;
	a.pb(CMMS_Type(1));
	a.pb(CMMS_Type(2));
	a.pb(CMMS_Type(3));
	Stree<CMMS_Type, CMMS_Upd> tree(a);
}

TEST(Manual_test, get)
{
	vector<CMMS_Type> a;
	a.pb(CMMS_Type(1));
	a.pb(CMMS_Type(2));
	a.pb(CMMS_Type(3));
	Stree<CMMS_Type, CMMS_Upd> tree(a);
	EXPECT_EQ(CMMS_Type(1), tree.get(1, 1));
	EXPECT_EQ(CMMS_Type(2), tree.get(2, 2));
	EXPECT_EQ(CMMS_Type(3), tree.get(3, 3));
	EXPECT_EQ(CMMS_Type(1, 2, 3), tree.get(1, 2));
	EXPECT_EQ(CMMS_Type(1, 3, 6), tree.get(1, 3));
	EXPECT_EQ(CMMS_Type(2, 3, 5), tree.get(2, 3));
}

TEST(Manual_test, update)
{
	vector<CMMS_Type> a;
	a.pb(CMMS_Type(1));
	a.pb(CMMS_Type(5));
	a.pb(CMMS_Type(3));
	a.pb(CMMS_Type(4));
	a.pb(CMMS_Type(2));
	Stree<CMMS_Type, CMMS_Upd> tree(a);
	tree.update(1, 3, CMMS_Upd(1));
	EXPECT_EQ(CMMS_Type(1, 1, 3), tree.get(1, 3));
	EXPECT_EQ(CMMS_Type(1, 4, 6), tree.get(2, 4));
	EXPECT_EQ(CMMS_Type(1, 4, 7), tree.get(3, 5));
	EXPECT_EQ(CMMS_Type(1, 4, 7), tree.get(1, 4));
	EXPECT_EQ(CMMS_Type(1, 4, 8), tree.get(2, 5));
	EXPECT_EQ(CMMS_Type(1, 4, 9), tree.get(1, 5));
}

TEST(Manual_test, Small_test)
{
	vector<CMMS_Type> a;
	for(int i = 0; i < 6; i++)
		a.pb(CMMS_Type(0));
	Stree<CMMS_Type, CMMS_Upd> tree(a);
	
	tree.update(2, 3, CMMS_Upd(1));
	tree.update(1, 4, CMMS_Upd(1));
	
	EXPECT_EQ(CMMS_Type(1, 1, 4), tree.get(1, 4));
	EXPECT_EQ(CMMS_Type(1, 1, 2), tree.get(2, 3));
	
	tree.update(1, 3, CMMS_Upd(2));
	tree.update(4, 6, CMMS_Upd(3));
	tree.update(3, 4, CMMS_Upd(-2));
	
	EXPECT_EQ(CMMS_Type(-2, 3, 6), tree.get(1, 6));
	EXPECT_EQ(CMMS_Type(2, 2, 4), tree.get(1, 2));
	EXPECT_EQ(CMMS_Type(-2, 3, 4), tree.get(4, 6));
	EXPECT_EQ(CMMS_Type(-2, -2, -4), tree.get(3, 4));
}

TEST(Stress_test, test)
{
	vector<CMMS_Type> vec;
	for(int i = 0; i < 1000; i++)
		vec.pb(CMMS_Type(0));
	Stree<CMMS_Type, CMMS_Upd> tree(vec);
	int a[1001];
	for(int i = 1; i <= 1000; i++) {
		a[i] = 0;
	}	
	for(int i = 0; i < 100000; i++) 
	{
		int x = rand()%1000 + 1;
		int y = x + rand()%(1000 - x + 1);
		if (rand()%2) {
			int t = rand()%10000;
			tree.update(x, y, CMMS_Upd(t));
			for(int i = x; i <= y; i++) {
				a[i] = t;
			}
		} else {
			MMS ans(INT_MAX, INT_MIN, 0);
			for(int i = x; i <= y; i++)
			{
				ans.sum += a[i];
				ans.min = min(ans.min, a[i]);
				ans.max = max(ans.max, a[i]);
			}
			ASSERT_EQ(tree.get(x, y).get_MMS(), ans);
		}
		ASSERT_TRUE(tree.check());
	}
}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}