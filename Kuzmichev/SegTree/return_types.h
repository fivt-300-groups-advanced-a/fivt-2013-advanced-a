#include "gtest/gtest.h"
#include <iostream>

using namespace std;

#ifndef RETURN_TYPES_DECL
#define RETURN_TYPES_DECL

struct StructSumMinMax
{
	const static int inf = 1e9;
	int sum;
	int min;
	int max;
	StructSumMinMax(int _sum, int _min, int _max) : sum(_sum), min(_min), max(_max) {}
	StructSumMinMax()
	{
		sum = 0;
		min = inf;
		max = -inf;
	}
	static StructSumMinMax merge(const StructSumMinMax L, const StructSumMinMax R)
	{
		return StructSumMinMax(L.sum + R.sum, std::min(L.min, R.min), std::max(L.max, R.max));
	}
	bool operator == (StructSumMinMax an) const
	{
		return sum == an.sum && min == an.min && max == an.max;
	}
};

struct StructConstancySegments
{
	int answer;
	int leftest;
	int rightest;
	StructConstancySegments()
	{
		answer = 1;
		leftest = rightest = 0;
	}
	StructConstancySegments(int _answer, int _leftest, int _rightest) : answer(_answer), leftest(_leftest), rightest(_rightest) {}
	static StructConstancySegments merge(const StructConstancySegments L, const StructConstancySegments R)
	{
		//printf("merge (%d %d)  (%d %d)\n", L.first, L.second, R.first, R.second);
		int newAnswer = L.answer + R.answer;
		if (L.rightest == R.leftest)
			newAnswer--;
		return StructConstancySegments(newAnswer, L.leftest, R.rightest);
	}
};

struct StructMaxSubarray
{
	static const int inf = 1e9;

	int maxSubarray;
	int maxPrefix;
	int maxSuffix;
	int sum;
	StructMaxSubarray()
	{
		maxSubarray = maxPrefix = maxSuffix = sum = 0;
	}
	StructMaxSubarray(int _maxSubarray, int _maxPrefix, int _maxSuffix, int _sum) : maxSubarray(_maxSubarray), maxPrefix(_maxPrefix), maxSuffix(_maxSuffix), sum (_sum){}
	void clear()
	{
		maxSubarray = maxPrefix = maxSuffix = sum = 0;
	}
	static StructMaxSubarray merge(const StructMaxSubarray L, const StructMaxSubarray R)
	{
		int newMaxSubarray = max(L.maxSubarray, max(R.maxSubarray, L.maxSuffix + R.maxPrefix));
		int newMaxPrefix = max(L.maxPrefix, L.sum + R.maxPrefix);
		int newMaxSuffix = max(R.maxSuffix, R.sum + L.maxSuffix);
		//return StructMaxSubarray(max(0, newMaxSubarray), max(0, newMaxPrefix), max(0, newMaxSuffix), L.sum + R.sum);
		return StructMaxSubarray(newMaxSubarray, newMaxPrefix, newMaxSuffix, L.sum + R.sum);
	}
};

#endif