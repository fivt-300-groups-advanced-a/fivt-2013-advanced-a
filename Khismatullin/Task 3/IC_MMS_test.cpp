#define TESTER

#include "gtest/gtest.h"
#include "IC_MMS_Stree.h"
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
using namespace std;

#define pb push_back

bool operator ==(const ICMMS_Type &a, const ICMMS_Type &b)
{
	return (a.get_min() == b.get_min() && a.get_max() == b.get_max() && a.get_sum() == b.get_sum());
}

bool operator ==(const MMS &a, const MMS &b)
{
	return (a.min == b.min && a.max == b.max && a.sum == b.sum);
}

TEST(Manual_test, build)
{
	vector<ICMMS_Type> a;
	a.pb(ICMMS_Type(1));
	a.pb(ICMMS_Type(2));
	a.pb(ICMMS_Type(3));
	Stree<ICMMS_Type, ICMMS_Upd> tree(a);
}

TEST(Manual_test, get)
{
	vector<ICMMS_Type> a;
	a.pb(ICMMS_Type(1));
	a.pb(ICMMS_Type(2));
	a.pb(ICMMS_Type(3));
	Stree<ICMMS_Type, ICMMS_Upd> tree(a);
	EXPECT_EQ(ICMMS_Type(1), tree.get(1, 1));
	EXPECT_EQ(ICMMS_Type(2), tree.get(2, 2));
	EXPECT_EQ(ICMMS_Type(3), tree.get(3, 3));
	EXPECT_EQ(ICMMS_Type(1, 2, 3), tree.get(1, 2));
	EXPECT_EQ(ICMMS_Type(1, 3, 6), tree.get(1, 3));
	EXPECT_EQ(ICMMS_Type(2, 3, 5), tree.get(2, 3));
}

TEST(Manual_test, update)
{
	vector<ICMMS_Type> a;
	a.pb(ICMMS_Type(1));
	a.pb(ICMMS_Type(5));
	a.pb(ICMMS_Type(3));
	a.pb(ICMMS_Type(4));
	a.pb(ICMMS_Type(2));
	Stree<ICMMS_Type, ICMMS_Upd> tree(a);
	tree.update(1, 5, ICMMS_Upd(1));
	EXPECT_EQ(ICMMS_Type(2, 6, 12), tree.get(1, 3));
	EXPECT_EQ(ICMMS_Type(4, 6, 15), tree.get(2, 4));
	EXPECT_EQ(ICMMS_Type(3, 5, 12), tree.get(3, 5));
	EXPECT_EQ(ICMMS_Type(2, 6, 17), tree.get(1, 4));
	EXPECT_EQ(ICMMS_Type(3, 6, 18), tree.get(2, 5));
	EXPECT_EQ(ICMMS_Type(2, 6, 20), tree.get(1, 5));
}

TEST(Manual_test, Small_test_Inc)
{
	vector<ICMMS_Type> a;
	for(int i = 0; i < 6; i++)
		a.pb(ICMMS_Type(0));
	Stree<ICMMS_Type, ICMMS_Upd> tree(a);
	
	tree.update(2, 3, ICMMS_Upd(1));
	tree.update(1, 4, ICMMS_Upd(-1));
	tree.update(1, 4, ICMMS_Upd(1));
	
	EXPECT_EQ(ICMMS_Type(0, 1, 2), tree.get(1, 4));
	EXPECT_EQ(ICMMS_Type(1, 1, 2), tree.get(2, 3));
	
	tree.update(2, 3, ICMMS_Upd(-1));
	tree.update(1, 3, ICMMS_Upd(2));
	tree.update(4, 6, ICMMS_Upd(3));
	tree.update(3, 4, ICMMS_Upd(-2));
	
	EXPECT_EQ(ICMMS_Type(0, 3, 11), tree.get(1, 6));
	EXPECT_EQ(ICMMS_Type(2, 2, 4), tree.get(1, 2));
	EXPECT_EQ(ICMMS_Type(1, 3, 7), tree.get(4, 6));
	EXPECT_EQ(ICMMS_Type(0, 1, 1), tree.get(3, 4));
}

TEST(Manual_test, Small_test_Ch)
{
	vector<ICMMS_Type> a;
	for(int i = 0; i < 6; i++)
		a.pb(ICMMS_Type(0));
	Stree<ICMMS_Type, ICMMS_Upd> tree(a);
	
	tree.update(2, 3, ICMMS_Upd(1, true));
	tree.update(1, 4, ICMMS_Upd(1, true));
	
	EXPECT_EQ(ICMMS_Type(1, 1, 4), tree.get(1, 4));
	EXPECT_EQ(ICMMS_Type(1, 1, 2), tree.get(2, 3));
	
	tree.update(1, 3, ICMMS_Upd(2, true));
	tree.update(4, 6, ICMMS_Upd(3, true));
	tree.update(3, 4, ICMMS_Upd(-2, true));
	
	EXPECT_EQ(ICMMS_Type(-2, 3, 6), tree.get(1, 6));
	EXPECT_EQ(ICMMS_Type(2, 2, 4), tree.get(1, 2));
	EXPECT_EQ(ICMMS_Type(-2, 3, 4), tree.get(4, 6));
	EXPECT_EQ(ICMMS_Type(-2, -2, -4), tree.get(3, 4));
}

TEST(Manual_test, Small_test)
{
	vector<ICMMS_Type> a;
	for(int i = 0; i < 7; i++)
		a.pb(ICMMS_Type(0));
	Stree<ICMMS_Type, ICMMS_Upd> tree(a);
	
	tree.update(3, 5, ICMMS_Upd(1, true));
	tree.update(1, 7, ICMMS_Upd(1));
	tree.update(1, 3, ICMMS_Upd(1));
	
	EXPECT_EQ(MMS(2, 3, 11), tree.get(1, 5).get_MMS());
	EXPECT_EQ(ICMMS_Type(1, 3, 13), tree.get(1, 7));
	EXPECT_EQ(ICMMS_Type(1, 2, 4), tree.get(5, 7));
}

TEST(Stress_test, test)
{
	vector<ICMMS_Type> vec;
	for(int i = 0; i < 1000; i++)
		vec.pb(ICMMS_Type(0));
	Stree<ICMMS_Type, ICMMS_Upd> tree(vec);
	int a[1001];
	for(int i = 1; i <= 1000; i++) {
		a[i] = 0;
	}	
	for(int i = 0; i < 100000; i++) 
	{
		int x = rand()%1000 + 1;
		int y = x + rand()%(1000 - x + 1);
		if (rand()%4 < 2) {
			int t = rand()%10000;
			if (rand()%2){
				tree.update(x, y, ICMMS_Upd(t, false));
				for(int i = x; i <= y; i++) {
					a[i] += t;
				}
			} else {
				tree.update(x, y, ICMMS_Upd(t, true));
				for(int i = x; i <= y; i++) {
					a[i] = t;
				}
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