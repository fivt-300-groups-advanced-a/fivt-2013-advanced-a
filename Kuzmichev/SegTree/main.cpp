#include <algorithm>
#include <iostream>
#include <cstdlib>
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
#include "assign_sum_min_max_tree.h"
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
	MethodsAssignSumMinMax M;
	SegTree <StructSumMinMax, MetaAssign , MethodsAssignSumMinMax> T(M, sz, StructSumMinMax(0, 0, 0));
	T.segOperation(0, 2, MetaAssign(true, 1));
	T.segOperation(2, 4, MetaAssign(true, 8));
	
	StructSumMinMax getResult = T.get(1, 4);
	ASSERT_EQ(getResult.sum, 25);
	ASSERT_EQ(getResult.min, 1);
	ASSERT_EQ(getResult.max, 8);
}

TEST(stresses, stress_assign)
{
	int sz = 100;
	int delta = 1000;
	vector <int> a;
	a.resize(sz + 5);
	MethodsAssignSumMinMax M;
	SegTree <StructSumMinMax, MetaAssign , MethodsAssignSumMinMax> T(M, sz, StructSumMinMax(0, 0, 0));
	for (int it = 0; it < 10000; it++)
	{
		int L, R;
		int typ = rand() % 2;
		
		if (typ == 0)
		{
			getLR(L, R, sz);
			int val = rand() % delta;
			if (DEBUG) printf("assign %d %d %d\n", L, R, val);
			fore(pos, L, R)
				a[pos] = val;
			T.segOperation(L, R, MetaAssign(true, val));
		}
		else if (typ == 1)
		{		
			getLR(L, R, sz);
			if (DEBUG) printf("get %d %d\n", L, R);
			
			StructSumMinMax right_ans;
			for (int i = L; i <= R; i++)
			{
				right_ans = StructSumMinMax(right_ans.sum + a[i], min(right_ans.min, a[i]), max(right_ans.max, a[i]));
			}
			StructSumMinMax tree_res = T.get(L, R);
			//if (DEBUG) printf("(%d %d) ?= (%d %d)\n", tree_res.fi, tree_res.se, right_res.fi, right_res.se);
			ASSERT_EQ(right_ans, tree_res);
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
	int delta = 100;
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
			int val = rand() % delta - delta / 2;
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
	int delta = 100;
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
			int val = rand() % delta - delta / 2;
			if (DEBUG3) printf("plus %d %d %d\n", L, R, val);
			fore(pos, L, R)
				a[pos] += val;
			T.segOperation(L, R, MetaPlusAssign(false, 0, val));
		}
		else if (typ == 1)
		{
			int val = rand() % delta - delta / 2;
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
	int delta = 100;
	vector <int> a;
	a.resize(sz + 5);
	MethodsConstancySegments M;
	
	SegTree <StructConstancySegments, MetaPlusAssign, MethodsConstancySegments> T(M, sz, StructConstancySegments(1, 0, 0));
	for (int it = 0; it < 10000; it++)
	{
		if (DEBUG3) printf("it = %d\n", it);
		int L, R;
		int typ = rand() % 3;
		
		if (typ == 0)
		{
			getLR(L, R, sz);
			int val = rand() % delta - delta / 2;
			if (DEBUG3) printf("assign %d %d %d\n", L, R, val);
			fore(pos, L, R)
				a[pos] = val;
			T.segOperation(L, R, MetaPlusAssign(true, val, 0));
		}
		else if (typ == 1)
		{
			getLR(L, R, sz);
			int val = rand() % delta - delta / 2;
			if (DEBUG3) printf("plus %d %d %d\n", L, R, val);
			fore(pos, L, R)
				a[pos] += val;
			T.segOperation(L, R, MetaPlusAssign(false, 0, val));
		}
		else if (typ == 2)
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



TEST( manual_tests, manual_test_6)
{
	int sz = 5;
	MethodsMaxSubarray M;
	SegTree <StructMaxSubarray, MetaAssign, MethodsMaxSubarray> T(M, sz, StructMaxSubarray(0, 0, 0, 0));
	T.segOperation(0, 1, MetaAssign(true, 25));
	T.segOperation(1, 3, MetaAssign(true, 8));
	StructMaxSubarray res = T.get(3, 4);
	res = T.get(0, 3);
	T.segOperation(0, 1, MetaAssign(true, -11));
	res = T.get(0, 2);
	res = T.get(0, 3);
	ASSERT_EQ(res.sum, -6);
	ASSERT_EQ(res.maxSuffix, 16);
	ASSERT_EQ(res.maxPrefix, 0);
	ASSERT_EQ(res.maxSubarray, 16);
}

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
	MethodsAssignSumMinMax M;
	SegTree <StructSumMinMax , MetaAssign , MethodsAssignSumMinMax> T(M, sz, StructSumMinMax(0, 0, 0));
	T.segOperation(1, 1, MetaAssign(true, 100));
	
	StructSumMinMax getResult = T.get(0, 1);
	
	ASSERT_EQ(getResult, StructSumMinMax(100, 0, 100));
	T.segOperation(0, 0, MetaAssign(true, 50));
	getResult = T.get(0, 1);
	ASSERT_EQ(getResult, StructSumMinMax(150, 50, 100));
	T.segOperation(0, 1, MetaAssign(true, 100500));
	getResult = T.get(0, 1);
	ASSERT_EQ(getResult, StructSumMinMax(100500 * 2, 100500, 100500));
}

TEST (manual_tests, manual_test_2)
{
	int sz = 3;
	MethodsAssignSumMinMax M;
	SegTree <StructSumMinMax, MetaAssign , MethodsAssignSumMinMax> T(M, sz, StructSumMinMax(0, 0, 0));
	T.segOperation(0, 0, MetaAssign(true, 1));
	T.segOperation(0, 2, MetaAssign(true, 8));
	
	StructSumMinMax getResult = T.get(0, 0);
	
	ASSERT_EQ(getResult, StructSumMinMax(8, 8, 8));
}

TEST(stresses, stress_max_subarray)
{
	int sz = 100;
	int delta = 1000;
	vector <int> a;
	vector <int> part_sum;
	a.resize(sz + 5);
	part_sum.resize(sz + 5);
	MethodsMaxSubarray M;
	SegTree <StructMaxSubarray, MetaAssign, MethodsMaxSubarray> T(M, sz, StructMaxSubarray(0, 0, 0, 0));
	for (int it = 0; it < 10000; it++)
	{
		int L, R;
		int typ = rand() % 2;
		
		if (typ == 0)
		{
			getLR(L, R, sz);
			int val = rand() % delta - delta * 2 / 3;
			if (DEBUG)
				printf("assign %d %d %d\n", L, R, val);
			fore(pos, L, R)
				a[pos] = val;
			T.segOperation(L, R, MetaAssign(true, val));
		}
		else if (typ == 1)
		{		
			getLR(L, R, sz);
			if (DEBUG)
				printf("get %d %d\n", L, R);
			
			int right_ans = 0;
			part_sum[L] = a[L];
			for (int i = L + 1; i <= R; i++)
				part_sum[i] = part_sum[i - 1] + a[i];
			for (int i = L; i <= R; i++)
			{
				for (int j = i; j <= R; j++)
					right_ans = max(right_ans, part_sum[j] - part_sum[i] + a[i]);
			}
			StructMaxSubarray tree_res = T.get(L, R);
			if (DEBUG)
				printf("%d ?= %d\n", tree_res.maxSubarray, right_ans);
			ASSERT_EQ(right_ans, tree_res.maxSubarray);
		}
	}
}

int main(int argc, char ** argv)
{
	testing::InitGoogleTest(&argc, argv); 
    RUN_ALL_TESTS(); 
}