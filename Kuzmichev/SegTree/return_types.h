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

#endif