#ifndef EXPRESSIONMAXIMISTION_H
#define EXPRESSIONMAXIMISTION_H

#include <vector>

template<typename DataType, typename InputIterator>
	DataType maximizeExpression(InputIterator first, InputIterator last)
{
	std::vector<DataType> prefs, dp(1, DataType(0));
	for (; first != last;)
	{
		DataType number = *(first++);
		for (size_t i = 0; i < prefs.size(); ++i)
			prefs[i] *= number;
		prefs.push_back(number);
		DataType value = dp.back() + number;
		for (size_t i = 2; i <= dp.size(); ++i)
			value = std::max(value, dp[dp.size() - i] + prefs[prefs.size() - i]);
		dp.push_back(value);
	}
	return dp.back();
}

template<typename DataType, typename InputIterator>
	DataType maximizeExpressionDummy(InputIterator first, InputIterator last)
{
	std::vector<DataType> v(first, last);
	int n = v.size();
	DataType answer(0);
	for (int msk = 0; msk < (1 << (n - 1)); ++msk)
	{
		DataType current(0);
		for (int i = 0; i < n;)
		{
			int j = i;
			while (j < n - 1 && (msk >> j) & 1) ++j;
			DataType prod = v[i];
			for (int z = i + 1; z <= j; ++z)
				prod *= v[z];
			current += prod;
			i = j + 1;
		}
		answer = std::max(answer, current);
	}
	return answer;
}

#endif // EXPRESSIONMAXIMISTION_H
