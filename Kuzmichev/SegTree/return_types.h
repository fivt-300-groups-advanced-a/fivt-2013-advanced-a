#include "gtest/gtest.h"

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
};

#endif