#define TESTER
#include "gtest/gtest.h"
#include "SubSeg_Tree.h"
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
using namespace std;

#define pb push_back

bool operator ==(int b, const SubS_Type &a)
{
	return (a.get_ans() == b);
}

TEST(Manual_test, build)
{
	vector<SubS_Type> a;
	a.pb(SubS_Type(1));
	a.pb(SubS_Type(2));
	a.pb(SubS_Type(3));
	Stree<SubS_Type, SubS_Upd> tree(a);
}

TEST(Manual_test, get)
{
	vector<SubS_Type> a;
	a.pb(SubS_Type(-1));
	a.pb(SubS_Type(0));
	a.pb(SubS_Type(2));
	Stree<SubS_Type, SubS_Upd> tree(a);
	EXPECT_EQ(0, tree.get(1, 1).get_ans());
	EXPECT_EQ(0, tree.get(2, 2));
	EXPECT_EQ(2, tree.get(3, 3));
	EXPECT_EQ(0, tree.get(1, 2));
	EXPECT_EQ(2, tree.get(1, 3));
	EXPECT_EQ(2, tree.get(2, 3));
}

TEST(Manual_test, update)
{
	vector<SubS_Type> a;
	a.pb(SubS_Type(3));
	a.pb(SubS_Type(-2));
	a.pb(SubS_Type(3));
	a.pb(SubS_Type(5));
	a.pb(SubS_Type(-2));
	Stree<SubS_Type, SubS_Upd> tree(a);
	tree.update(2, 3, SubS_Upd(1));
	EXPECT_EQ(5, tree.get(1, 3));
	EXPECT_EQ(7, tree.get(2, 4));
	EXPECT_EQ(6, tree.get(3, 5));
	EXPECT_EQ(10, tree.get(1, 4));
	EXPECT_EQ(7, tree.get(2, 5));
	EXPECT_EQ(10, tree.get(1, 5));
}

TEST(Manual_test, Small_test)
{
	vector<SubS_Type> a;
	for(int i = 0; i < 6; i++)
		a.pb(SubS_Type(0));
	Stree<SubS_Type, SubS_Upd> tree(a);
	
	tree.update(1, 4, SubS_Upd(-1));
	tree.update(2, 3, SubS_Upd(1));
	
	EXPECT_EQ(2, tree.get(1, 4));
	EXPECT_EQ(2, tree.get(2, 3));
	EXPECT_EQ(2, tree.get(1, 6).get_ans());
	EXPECT_EQ(1, tree.get(3, 5).get_ans());
	
	tree.update(1, 3, SubS_Upd(3));
	tree.update(4, 6, SubS_Upd(3));
	tree.update(3, 4, SubS_Upd(-2));
	
	EXPECT_EQ(8, tree.get(1, 6));
	EXPECT_EQ(6, tree.get(1, 2));
	EXPECT_EQ(6, tree.get(4, 6));
	EXPECT_EQ(0, tree.get(3, 4));
}

TEST(Stress_test, test)
{
	vector<SubS_Type> vec;
	for(int i = 0; i < 1000; i++)
		vec.pb(SubS_Type(0));
	Stree<SubS_Type, SubS_Upd> tree(vec);
	int a[1001];
	for(int i = 1; i <= 1000; i++) {
		a[i] = 0;
	}	
	for(int i = 0; i < 100000; i++) 
	{
		int x = rand()%1000 + 1;
		int y = x + rand()%(1000 - x + 1);
		//int y = x;
		if (rand()%2) {
			int t = rand()%10000 - 5000;
			//cout << "+ " << x << " " << y << " " << t << endl;
			tree.update(x, y, SubS_Upd(t));
			for(int i = x; i <= y; i++) {
				a[i] = t;
			}
		} else {
			int ans = 0;
			for(int l = x; l <= y; l++) {
				int sum = 0;
				for(int r = l; r <= y; r++)
				{
					sum += a[r];
					ans = max(ans, sum);
				}
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