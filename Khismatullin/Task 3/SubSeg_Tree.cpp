#define TESTER
#include "gtest/gtest.h"
#include "Per_Stree.h"
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
using namespace std;

#define pb push_back

bool operator ==(int b, const Per_Type &a)
{
	return (a.get_ans() == b);
}

TEST(Manual_test, build)
{
	vector<Per_Type> a;
	a.pb(Per_Type(1));
	a.pb(Per_Type(2));
	a.pb(Per_Type(3));
	Stree<Per_Type, Per_Upd> tree(a);
}

TEST(Manual_test, get)
{
	vector<Per_Type> a;
	a.pb(Per_Type(1));
	a.pb(Per_Type(2));
	a.pb(Per_Type(3));
	Stree<Per_Type, Per_Upd> tree(a);
	EXPECT_EQ(1, tree.get(1, 1).get_ans());
	EXPECT_EQ(1, tree.get(2, 2));
	EXPECT_EQ(1, tree.get(3, 3));
	EXPECT_EQ(2, tree.get(1, 2));
	EXPECT_EQ(3, tree.get(1, 3));
	EXPECT_EQ(2, tree.get(2, 3));
}

TEST(Manual_test, update)
{
	vector<Per_Type> a;
	a.pb(Per_Type(1));
	a.pb(Per_Type(2));
	a.pb(Per_Type(3));
	a.pb(Per_Type(4));
	a.pb(Per_Type(5));
	Stree<Per_Type, Per_Upd> tree(a);
	tree.update(1, 3, Per_Upd(1));
	EXPECT_EQ(1, tree.get(1, 3));
	EXPECT_EQ(2, tree.get(2, 4));
	EXPECT_EQ(3, tree.get(3, 5));
	EXPECT_EQ(2, tree.get(1, 4));
	EXPECT_EQ(3, tree.get(2, 5));
	EXPECT_EQ(3, tree.get(1, 5));
}

TEST(Manual_test, Small_test)
{
	vector<Per_Type> a;
	for(int i = 0; i < 6; i++)
		a.pb(Per_Type(0));
	Stree<Per_Type, Per_Upd> tree(a);
	
	tree.update(1, 4, Per_Upd(1));
	tree.update(2, 3, Per_Upd(2));
	
	EXPECT_EQ(3, tree.get(1, 4));
	EXPECT_EQ(1, tree.get(2, 3));
	EXPECT_EQ(4, tree.get(1, 6).get_ans());
	EXPECT_EQ(3, tree.get(3, 5).get_ans());
	
	tree.update(1, 3, Per_Upd(2));
	tree.update(4, 6, Per_Upd(3));
	tree.update(3, 4, Per_Upd(1));
	
	EXPECT_EQ(3, tree.get(1, 6));
	EXPECT_EQ(1, tree.get(1, 2));
	EXPECT_EQ(2, tree.get(4, 6));
	EXPECT_EQ(1, tree.get(3, 4));
}

TEST(Stress_test, test)
{
	vector<Per_Type> vec;
	for(int i = 0; i < 1000; i++)
		vec.pb(Per_Type(0));
	Stree<Per_Type, Per_Upd> tree(vec);
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
			tree.update(x, y, Per_Upd(t));
			for(int i = x; i <= y; i++) {
				a[i] = t;
			}
		} else {
			int i = x, ans = 0;
			while (i <= y) {
				int l = i;
				while (i <= y && a[l] == a[i]) i++;
				ans++;
			}
			ASSERT_EQ(tree.get(x, y).get_ans(), ans);
		}
		ASSERT_TRUE(tree.check());
	}
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}