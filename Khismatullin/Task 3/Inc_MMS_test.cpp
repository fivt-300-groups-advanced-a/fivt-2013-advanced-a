#define TESTER
#include "gtest/gtest.h"
#include "Inc_MMS_Stree.h"
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
using namespace std;

#define pb push_back

bool operator ==(const IMMS_Type &a, const IMMS_Type &b)
{
	return (a.get_min() == b.get_min() && a.get_max() == b.get_max() && a.get_sum() == b.get_sum());
}

bool operator ==(const MMS &a, const MMS &b)
{
	return (a.min == b.min && a.max == b.max && a.sum == b.sum);
}

TEST(Unit_test, constructor)
{
	Stree<IMMS_Type, IMMS_Upd> tree;
	EXPECT_TRUE(tree.is_empty());
	Stree<IMMS_Type, IMMS_Upd> tree_2(0);
	EXPECT_TRUE(tree_2.is_empty());
	Stree<IMMS_Type, IMMS_Upd> tree_3(1);
	EXPECT_FALSE(tree_3.is_empty());
}

TEST(Unit_test, resize)
{
	Stree<IMMS_Type, IMMS_Upd> tree;
	for(int n = 1; n <= 100; n++)
	{
		tree.resize(n);
		EXPECT_FALSE(tree.is_empty());
		for(int i = 1; i <= n; i++)
			EXPECT_EQ(IMMS_Type(), tree.get(i, i));
	}
}

TEST(Unit_test, clear)
{
	Stree<IMMS_Type, IMMS_Upd> tree;
	for(int n = 1; n <= 100; n++)
	{
		tree.resize(n);
		EXPECT_FALSE(tree.is_empty());
		tree.clear();
		EXPECT_TRUE(tree.is_empty());
	}
}

TEST(Unit_test, build)
{
	for(int n = 1; n <= 5; n++)
	{
		vector<IMMS_Type> a;
		for(int i = 0; i < n; i++)
			a.pb(IMMS_Type(rand()%10, rand()%10, rand()%10));
		Stree<IMMS_Type, IMMS_Upd> tree(a);
		for(int i = 0; i < n; i++)
			EXPECT_EQ(a[i], tree.get(i + 1, i + 1));
	}
}

TEST(Unit_test, update)
{
	vector<IMMS_Type> v;
	v.pb(IMMS_Type(0));
	Stree<IMMS_Type, IMMS_Upd> tree(v);
	for(int i = 1; i <= 10; i++)
		tree.update(1, 1, IMMS_Upd(i));
	EXPECT_EQ(IMMS_Type(55), tree.get(1, 1));
}


TEST(Unit_test, push)
{
	vector<IMMS_Type> v;
	v.pb(IMMS_Type(0));
	v.pb(IMMS_Type(0));
	Stree<IMMS_Type, IMMS_Upd> tree(v);
	for(int i = 1; i <= 10; i++)
		tree.update(1, 2, IMMS_Upd(i));
	EXPECT_EQ(IMMS_Type(55), tree.get(1, 1));
	EXPECT_EQ(IMMS_Type(55), tree.get(2, 2));
	EXPECT_EQ(IMMS_Type(55, 55, 110), tree.get(1, 2));
}


TEST(Unit_test, get)
{
	for(int n = 1; n < 100; n++)
	{
		vector<IMMS_Type> v;
		for(int i = 0; i < n; i++)
			v.pb(rand()%100);
		Stree<IMMS_Type, IMMS_Upd> tree(v);
		for(int l = 1; l <= n; l++)
			for(int r = l; r <= n; r++)
			{
				MMS ans(INT_MAX, INT_MIN, 0);
				for(int i = l - 1; i < r; i++)
				{
					ans.sum += v[i].get_sum();
					ans.min = min(ans.min, v[i].get_sum());
					ans.max = max(ans.max, v[i].get_sum());
				}
				EXPECT_EQ(ans, tree.get(l, r).get_MMS());
			}
	}
}

TEST(Manual_test, build)
{
	vector<IMMS_Type> a;
	a.pb(IMMS_Type(1));
	a.pb(IMMS_Type(2));
	a.pb(IMMS_Type(3));
	Stree<IMMS_Type, IMMS_Upd> tree(a);
}

TEST(Manual_test, get)
{
	vector<IMMS_Type> a;
	a.pb(IMMS_Type(1));
	a.pb(IMMS_Type(2));
	a.pb(IMMS_Type(3));
	Stree<IMMS_Type, IMMS_Upd> tree(a);
	EXPECT_EQ(IMMS_Type(1), tree.get(1, 1));
	EXPECT_EQ(IMMS_Type(2), tree.get(2, 2));
	EXPECT_EQ(IMMS_Type(3), tree.get(3, 3));
	EXPECT_EQ(IMMS_Type(1, 2, 3), tree.get(1, 2));
	EXPECT_EQ(IMMS_Type(1, 3, 6), tree.get(1, 3));
	EXPECT_EQ(IMMS_Type(2, 3, 5), tree.get(2, 3));
}

TEST(Manual_test, update)
{
	vector<IMMS_Type> a;
	a.pb(IMMS_Type(1));
	a.pb(IMMS_Type(5));
	a.pb(IMMS_Type(3));
	a.pb(IMMS_Type(4));
	a.pb(IMMS_Type(2));
	Stree<IMMS_Type, IMMS_Upd> tree(a);
	tree.update(1, 5, IMMS_Upd(1));
	EXPECT_EQ(IMMS_Type(2, 6, 12), tree.get(1, 3));
	EXPECT_EQ(IMMS_Type(4, 6, 15), tree.get(2, 4));
	EXPECT_EQ(IMMS_Type(3, 5, 12), tree.get(3, 5));
	EXPECT_EQ(IMMS_Type(2, 6, 17), tree.get(1, 4));
	EXPECT_EQ(IMMS_Type(3, 6, 18), tree.get(2, 5));
	EXPECT_EQ(IMMS_Type(2, 6, 20), tree.get(1, 5));
}

TEST(Manual_test, Small_test)
{
	vector<IMMS_Type> a;
	for(int i = 0; i < 6; i++)
		a.pb(IMMS_Type(0));
	Stree<IMMS_Type, IMMS_Upd> tree(a);
	
	tree.update(2, 3, IMMS_Upd(1));
	tree.update(1, 4, IMMS_Upd(-1));
	tree.update(1, 4, IMMS_Upd(1));
	
	EXPECT_EQ(IMMS_Type(0, 1, 2), tree.get(1, 4));
	EXPECT_EQ(IMMS_Type(1, 1, 2), tree.get(2, 3));
	
	tree.update(2, 3, IMMS_Upd(-1));
	tree.update(1, 3, IMMS_Upd(2));
	tree.update(4, 6, IMMS_Upd(3));
	tree.update(3, 4, IMMS_Upd(-2));
	
	EXPECT_EQ(IMMS_Type(0, 3, 11), tree.get(1, 6));
	EXPECT_EQ(IMMS_Type(2, 2, 4), tree.get(1, 2));
	EXPECT_EQ(IMMS_Type(1, 3, 7), tree.get(4, 6));
	EXPECT_EQ(IMMS_Type(0, 1, 1), tree.get(3, 4));
}

TEST(Stress_manual_test, small_test)
{
	vector<IMMS_Type> vec;
	for(int i = 0; i < 10; i++)
		vec.pb(IMMS_Type(0));
	Stree<IMMS_Type, IMMS_Upd> tree(vec);
	int a[11];
	for(int i = 1; i <= 10; i++) {
		a[i] = 0;
	}	
	for(int i = 0; i < 100; i++) 
	{
		int x = rand()%10 + 1;
		int y = x + rand()%(10 - x + 1);
		if (rand()%2) {
			int t = rand()%5;
			tree.update(x, y, IMMS_Upd(t));
			for(int i = x; i <= y; i++) {
				a[i] += t;
			}
			//cout << "+ " <<x << " " << y << " " << t << endl;
		} else {
			MMS ans(INT_MAX, INT_MIN, 0);
			for(int i = x; i <= y; i++)
			{
				ans.sum += a[i];
				ans.min = min(ans.min, a[i]);
				ans.max = max(ans.max, a[i]);
			}
			//cout << x << " " << y << " " << ans.min << " " << ans.max << " " << ans.sum << endl;
			//cout << tree.get(x, y).get_min() << " " << tree.get(x, y).get_max() << " " << tree.get(x, y).get_sum() << endl;
			ASSERT_EQ(tree.get(x, y).get_MMS(), ans);
		}
		ASSERT_TRUE(tree.check());
	}
}

TEST(Stress_test, test)
{
	vector<IMMS_Type> vec;
	for(int i = 0; i < 1000; i++)
		vec.pb(IMMS_Type(0));
	Stree<IMMS_Type, IMMS_Upd> tree(vec);
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
			tree.update(x, y, IMMS_Upd(t));
			for(int i = x; i <= y; i++) {
				a[i] += t;
			}
		} else {
			MMS ans(INT_MAX, INT_MIN, 0);
			for(int i = x; i <= y; i++)
			{
				ans.sum += a[i];
				ans.min = min(ans.min, a[i]);
				ans.max = max(ans.max, a[i]);
			}
			//cout << x << " " << y << " " << ans.min << " " << ans.max << " " << ans.sum << endl;
			//cout << tree.get(x, y).get_min() << " " << tree.get(x, y).get_max() << " " << tree.get(x, y).get_sum() << endl;
			ASSERT_EQ(tree.get(x, y).get_MMS(), ans);
		}
		ASSERT_TRUE(tree.check());
	}
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}