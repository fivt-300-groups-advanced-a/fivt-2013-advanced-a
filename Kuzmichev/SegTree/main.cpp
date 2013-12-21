#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>
#include <vector>
#include <cstdio>
#include <set>
#include <map>
#include <queue>
#include <memory.h>
#include <cmath>
#include "segtree.h"
#include "return_types.h"
#include "assign_sum_min_tree.h"
#include "plus_sum_min_max_tree.h"
#include "max_subarray_tree.h"
#include "assign_plus_sum_min_max_tree.h"
#include "constancy_segments_tree.h"
#include "gtest/gtest.h"

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)n; i++)
#define fore(i, b, e) for (int i = (int)b; i <= (int)e; i++)
#define all(x) (x).begin(), (x).end()
#define mp make_pair
#define pb push_back
#define fi first
#define se second
typedef long long i64;

const int inf = 1e9;

inline void getLR(int & L, int & R, int sz)
{
	L = rand() % sz;
	R = rand() % sz;
	if (L > R)
		swap(L, R);
}

TEST (manual_tests, manual_test_3)
{
	int sz = 5;
	MethodsAssignSumMin M;
	SegTree <pair <int, int >, MetaAssign , MethodsAssignSumMin> T(M, sz, mp(0, 0));
	T.segOperation(0, 2, MetaAssign(true, 1));
	T.segOperation(2, 4, MetaAssign(true, 8));
	
	pair <int, int> getResult = T.get(1, 4);
	if (DEBUG) printf("%d %d\n", getResult.first, getResult.second);
	ASSERT_EQ(getResult, mp(25, 1));
}

TEST(stresses, stressAssignSumMin)
{
	int sz = 100;
	vector <int> a;
	a.resize(sz + 5);
	MethodsAssignSumMin M;
	SegTree <pair <int, int >, MetaAssign , MethodsAssignSumMin> T(M, sz, mp(0, 0));
	for (int it = 0; it < 10000; it++)
	{
		int L, R;
		int typ = rand() % 2;
		
		if (typ == 0)
		{
			getLR(L, R, sz);
			int val = rand() % 100;
			if (DEBUG) printf("assign %d %d %d\n", L, R, val);
			fore(pos, L, R)
				a[pos] = val;
			T.segOperation(L, R, MetaAssign(true, val));
		}
		else if (typ == 1)
		{		
			getLR(L, R, sz);
			if (DEBUG) printf("get %d %d\n", L, R);
			pair <int, int> tree_res = T.get(L, R);
			pair <int, int> right_res = mp(0, inf);
			for (int i = L; i <= R; i++)
			{
				right_res = mp(right_res.first + a[i], min(right_res.second, a[i]));
			}
			if (DEBUG) printf("(%d %d) ?= (%d %d)\n", tree_res.fi, tree_res.se, right_res.fi, right_res.se);
			ASSERT_EQ(right_res, tree_res);
		}
	}
}

TEST (manual_tests, manual_test_4)
{
	MethodsPlusSumMinMax M;
	
	SegTree <StructSumMinMax, MetaPlus, MethodsPlusSumMinMax> T(M, 4, StructSumMinMax(0, 0, 0));
	T.segOperation(1, 2, MetaPlus(2));
	StructSumMinMax res = T.get(0, 2);
	ASSERT_EQ(res.sum, 4); 
	ASSERT_EQ(res.min, 0);
	ASSERT_EQ(res.max, 2);
}

TEST(stresses, stress_plus)
{
	int sz = 100;	
	vector <int> a;
	a.resize(sz + 5);
	MethodsPlusSumMinMax M;
	
	SegTree <StructSumMinMax, MetaPlus, MethodsPlusSumMinMax> T(M, sz, StructSumMinMax(0, 0, 0));
	for (int it = 0; it < 10000; it++)
	{
		if (DEBUG3) printf("it = %d\n", it);
		int L, R;
		int typ = rand() % 2;
		
		if (typ == 0)
		{
			getLR(L, R, sz);
			int val = rand() % 100;
			if (DEBUG3) printf("plus %d %d %d\n", L, R, val);
			fore(pos, L, R)
				a[pos] += val;
			T.segOperation(L, R, MetaPlus(val));
		}
		else if (typ == 1)
		{		
			getLR(L, R, sz);
			if (DEBUG3) printf("get %d %d\n", L, R);
			StructSumMinMax tree_res = T.get(L, R);
			StructSumMinMax right_res = StructSumMinMax(0, inf, -inf); 
			for (int i = L; i <= R; i++)
			{
				right_res = StructSumMinMax(right_res.sum + a[i], min(right_res.min, a[i]), max(right_res.max, a[i]));
			}
			if (DEBUG3) printf("(%d %d %d) ?= (%d %d %d)\n", tree_res.sum, tree_res.min, tree_res.max, right_res.sum, right_res.min, right_res.max);
			ASSERT_EQ(right_res.sum, tree_res.sum);
			ASSERT_EQ(right_res.min, tree_res.min);
			ASSERT_EQ(right_res.max, tree_res.max);
		}
	}
}

TEST(stresses, stress_assign_and_plus)
{
	int sz = 100;	
	vector <int> a;
	a.resize(sz + 5);
	MethodsPlusAssignSumMinMax M;
	
	SegTree <StructSumMinMax, MetaPlusAssign, MethodsPlusAssignSumMinMax> T(M, sz, StructSumMinMax(0, 0, 0));
	for (int it = 0; it < 10000; it++)
	{
		if (DEBUG3) printf("it = %d\n", it);
		int L, R;
		int typ = rand() % 3;
		getLR(L, R, sz);
		if (typ == 0)
		{
			int val = rand() % 100;
			if (DEBUG3) printf("plus %d %d %d\n", L, R, val);
			fore(pos, L, R)
				a[pos] += val;
			T.segOperation(L, R, MetaPlusAssign(false, 0, val));
		}
		else if (typ == 1)
		{
			int val = rand() % 100;
			if (DEBUG3) printf("assign %d %d %d\n", L, R, val);
			fore(pos, L, R)
				a[pos] = val;
			T.segOperation(L, R, MetaPlusAssign(true, val, 0));
		}
		else if (typ == 2)
		{		
			if (DEBUG3) printf("get %d %d\n", L, R);
			StructSumMinMax tree_res = T.get(L, R);
			StructSumMinMax right_res = StructSumMinMax(0, inf, -inf); 
			for (int i = L; i <= R; i++)
			{
				right_res = StructSumMinMax(right_res.sum + a[i], min(right_res.min, a[i]), max(right_res.max, a[i]));
			}
			if (DEBUG3) printf("(%d %d %d) ?= (%d %d %d)\n", tree_res.sum, tree_res.min, tree_res.max, right_res.sum, right_res.min, right_res.max);
			ASSERT_EQ(right_res.sum, tree_res.sum);
			ASSERT_EQ(right_res.min, tree_res.min);
			ASSERT_EQ(right_res.max, tree_res.max);
		}
	}
}

TEST(stresses, stress_constancy_segments)
{
	int sz = 100;	
	vector <int> a;
	a.resize(sz + 5);
	MethodsConstancySegments M;
	
	SegTree <StructConstancySegments, MetaAssign, MethodsConstancySegments> T(M, sz, StructConstancySegments(1, 0, 0));
	for (int it = 0; it < 10000; it++)
	{
		if (DEBUG3) printf("it = %d\n", it);
		int L, R;
		int typ = rand() % 2;
		
		if (typ == 0)
		{
			getLR(L, R, sz);
			int val = rand() % 100;
			if (DEBUG3) printf("assign %d %d %d\n", L, R, val);
			fore(pos, L, R)
				a[pos] = val;
			T.segOperation(L, R, MetaAssign(true, val));
		}
		else if (typ == 1)
		{		
			getLR(L, R, sz);
			if (DEBUG3) printf("get %d %d\n", L, R);
			int right_ans = 1;
			for (int i = L + 1; i <= R; i++)
			{
				if (a[i] != a[i - 1])
					right_ans++;
			}
			StructConstancySegments tree_res = T.get(L, R);
			if (DEBUG3) printf("%d ?= %d\n", tree_res.answer, right_ans);
			ASSERT_EQ(right_ans, tree_res.answer);
			ASSERT_EQ(a[L], tree_res.leftest);
			ASSERT_EQ(a[R], tree_res.rightest);
		}
	}
}

/*TEST(stresses, stress_assign)
{
	int sz = 100;	
	vector <int> a;
	a.resize(sz + 5);
	Methods M;
	
	SegTree <pair <int, int>, MetaAssign , Methods> T(M, sz, mp(0, 0));
	for (int it = 0; it < 100000; it++)
	{
		int typ = rand() % 2;
		int L, R;
		if (typ == 0)
		{
			int pos = rand() % sz;
			int new_val = rand() % 100;
			T.updateInPosition(pos, mp(new_val, new_val));
			a[pos] = new_val;
			//printf("upd pos = %d val = %d\n", pos, new_val);
		}
		else if (typ == 1)
		{
			getLR(L, R, sz);
			//printf("get %d %d\n", L, R);
			pair <int, int> tree_res = T.get(L, R);
			pair <int, int> right_res = mp(0, inf);
			for (int i = L; i <= R; i++)
			{
				right_res = mp(right_res.first + a[i], min(right_res.second, a[i]));
			}
			ASSERT_EQ(tree_res, right_res);
		}
	}
}*/

TEST( manual_tests, manual_test_5)
{
	int sz = 5;
	MethodsPlusAssignSumMinMax M;
	SegTree <StructSumMinMax, MetaPlusAssign, MethodsPlusAssignSumMinMax> T(M, sz, StructSumMinMax(0, 0, 0));
	T.segOperation(1, 2, MetaPlusAssign(true, 100, 0));
	T.segOperation(0, 4, MetaPlusAssign(false, 0, 50));

	StructSumMinMax res = T.get(0, 4);
	ASSERT_EQ(res.sum, 450);
	ASSERT_EQ(res.min, 50);
	ASSERT_EQ(res.max, 150);
	
	T.segOperation(2, 3, MetaPlusAssign(true, 1000, 0));
	res = T.get(0, 4);
	ASSERT_EQ(res.sum, 2250);
	ASSERT_EQ(res.min, 50);
	ASSERT_EQ(res.max, 1000);

	res = T.get(1, 3);
	ASSERT_EQ(res.sum, 2150);
	ASSERT_EQ(res.min, 150);
	ASSERT_EQ(res.max, 1000);
}

TEST( manual_tests, manual_test_1)
{
	int sz = 2;
	MethodsAssignSumMin M;
	SegTree <pair <int, int >, MetaAssign , MethodsAssignSumMin> T(M, sz, mp(0, 0));
	T.segOperation(1, 1, MetaAssign(true, 100));
	
	pair <int, int> getResult = T.get(0, 1);
	if (DEBUG) printf("%d %d\n", getResult.first, getResult.second);
	assert(getResult == mp(100, 0));
	T.segOperation(0, 0, MetaAssign(true, 50));
	getResult = T.get(0, 1);
	if (DEBUG) printf("%d %d\n", getResult.first, getResult.second);
	assert(getResult == mp(150, 50));
	T.segOperation(0, 1, MetaAssign(true, 100500));
	getResult = T.get(0, 1);
	if (DEBUG) printf("%d %d\n", getResult.first, getResult.second);
	assert(getResult == mp(100500 * 2, 100500));
}

TEST (manual_tests, manual_test_2)
{
	int sz = 3;
	MethodsAssignSumMin M;
	SegTree <pair <int, int >, MetaAssign , MethodsAssignSumMin> T(M, sz, mp(0, 0));
	T.segOperation(0, 0, MetaAssign(true, 1));
	T.segOperation(0, 2, MetaAssign(true, 8));
	
	pair <int, int> getResult = T.get(0, 0);
	if (DEBUG) printf("%d %d\n", getResult.first, getResult.second);
	assert(getResult == mp(8, 8));
}

int main(int argc, char ** argv)
{
	testing::InitGoogleTest(&argc, argv); 
    RUN_ALL_TESTS(); 
}