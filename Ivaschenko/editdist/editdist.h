#ifndef EDITDIST_H
#define EDITDIST_H

#include <vector>
#include <algorithm>
#include <functional>

#include <cassert>

namespace
{

	template<typename RandomAccessIterator, typename Equal>
		std::vector<std::size_t> lcsPrefix(RandomAccessIterator aFirst, std::size_t aLeft, std::size_t aRight,
										   RandomAccessIterator bFirst, std::size_t bLeft, std::size_t bRight,
										   Equal comparator)
	{
		std::size_t m = bRight - bLeft + 1;
		std::size_t *dp[2];
		for (int i = 0; i < 2; ++i)
		{
			dp[i] = new std::size_t[m + 1];
			std::fill(dp[i], dp[i] + m + 1, 0);
		}
		for (std::size_t i = aLeft; i <= aRight; ++i)
		{
			for (std::size_t j = 0; j <= m; ++j)
			{
				dp[1][j] = std::max(dp[1][j], dp[0][j]);
				if (j + 1 <= m) dp[0][j + 1] = std::max(dp[0][j + 1], dp[0][j]);
				if (j + 1 <= m && comparator(*(aFirst + i), *(bFirst + bLeft + j)))
					dp[1][j + 1] = std::max(dp[1][j + 1], dp[0][j] + 1);
			}
			std::swap(dp[0], dp[1]);
			std::fill(dp[1], dp[1] + m + 1, 0);
		}
		for (std::size_t j = 0; j < m; ++j)
			dp[0][j + 1] = std::max(dp[0][j + 1], dp[0][j]);
		std::vector<std::size_t> answer(dp[0], dp[0] + m + 1);
		for (int i = 0; i < 2; ++i) delete dp[i];
		return answer;
	}

	template<typename RandomAccessIterator, typename Equal>
		std::vector<std::size_t> lcsSuffix(RandomAccessIterator aFirst, std::size_t aLeft, std::size_t aRight,
										   RandomAccessIterator bFirst, std::size_t bLeft, std::size_t bRight,
										   Equal comparator)
	{
		std::size_t m = bRight - bLeft + 1;
		std::size_t *dp[2];
		for (int i = 0; i < 2; ++i)
		{
			dp[i] = new std::size_t[m + 1];
			std::fill(dp[i], dp[i] + m + 1, 0);
		}
		for (std::size_t i = aRight; i >= aLeft; --i)
		{
			for (std::size_t j = m;	j >= 1; --j)
			{
				dp[1][j] = std::max(dp[1][j], dp[0][j]);
				if (j > 0) dp[0][j - 1] = std::max(dp[0][j - 1], dp[0][j]);
				if (j > 0 && comparator(*(aFirst + i), *(bFirst + bLeft + j - 1)))
					dp[1][j - 1] = std::max(dp[1][j - 1], dp[0][j] + 1);
			}
			dp[0][0] = std::max(dp[0][0], dp[0][1]);
			std::swap(dp[0], dp[1]);
			std::fill(dp[1], dp[1] + m + 1, 0);
		}
		for (std::size_t i = m; i >= 1; --i)
			dp[0][i - 1] = std::max(dp[0][i - 1], dp[0][i]);
		std::vector<std::size_t> answer(dp[0], dp[0] + m + 1);
		for (int i = 0; i < 2; ++i) delete dp[i];
		return answer;
	}

	template<typename RandomAccessIterator, typename Equal>
		std::vector<RandomAccessIterator> largestCommonSubsequence(
				RandomAccessIterator aFirst, RandomAccessIterator aLast,
				RandomAccessIterator bFirst, RandomAccessIterator bLast,
				Equal comparator)
	{
		std::size_t n = aLast - aFirst, m = bLast - bFirst;
		if (n == 1)
		{
			auto pos = std::find(bFirst, bLast, *aFirst);
			if (pos == bLast) return std::vector<RandomAccessIterator>();
			return std::vector<RandomAccessIterator>(1, pos);
		}
		std::size_t firstHalf = n / 2;
		std::size_t bestPartition = 0;
		{
			std::vector<std::size_t> first = lcsPrefix(aFirst, 0, firstHalf - 1, bFirst, 0, m - 1, comparator);
			std::vector<std::size_t> second = lcsSuffix(aFirst, firstHalf, n - 1, bFirst, 0, m - 1, comparator);
			for (std::size_t i = 0; i <= m; ++i)
				if (first[i] + second[i] > first[bestPartition] + second[bestPartition])
					bestPartition = i;
		}
		std::vector<RandomAccessIterator> left =
				largestCommonSubsequence(aFirst, aFirst + firstHalf,
										 bFirst, bFirst + bestPartition,
										 comparator);
		std::vector<RandomAccessIterator> right =
				largestCommonSubsequence(aFirst + firstHalf, aLast,
										 bFirst + bestPartition, bLast,
										 comparator);
		left.insert(left.end(), right.begin(), right.end());
		return left;
	}

	template<typename RandomAccessIterator, typename Equal>
		std::vector<RandomAccessIterator> dummyLargestCommonSubsequence(
				RandomAccessIterator aFirst, RandomAccessIterator aLast,
				RandomAccessIterator bFirst, RandomAccessIterator bLast,
				Equal comparator)
	{
		std::size_t n = aLast - aFirst, m = bLast - bFirst;
		std::size_t **dp = new std::size_t* [n + 1];
		for (std::size_t i = 0; i <= n; ++i)
		{
			dp[i] = new std::size_t[m + 1];
			std::fill(dp[i], dp[i] + m + 1, 0);
		}
		for (std::size_t i = 0; i <= n; ++i)
			for (std::size_t j = 0; j <= m; ++j)
			{
				if (i + 1 <= n) dp[i + 1][j] = std::max(dp[i + 1][j], dp[i][j]);
				if (j + 1 <= m) dp[i][j + 1] = std::max(dp[i][j + 1], dp[i][j]);
				if (i + 1 <= n && j + 1 <= m && comparator(*(aFirst + i), *(bFirst + j)))
					dp[i + 1][j + 1] = std::max(dp[i + 1][j + 1], dp[i][j] + 1);
			}
		std::size_t curn = n, curm = m;
		std::vector<RandomAccessIterator> answer;
		while (curn > 0 || curm > 0)
		{
			if (curn > 0 && dp[curn - 1][curm] == dp[curn][curm]) curn--;
			else if (curm > 0 && dp[curn][curm - 1] == dp[curn][curm]) curm--;
			else
			{
				assert(curn > 0 && curm > 0);
				--curn, --curm;
				assert(comparator(*(aFirst + curn), *(bFirst + curm)));
				answer.push_back(bFirst + curm);
			}
		}
		for (std::size_t i = 0; i <= n; ++i)
			delete dp[i];
		reverse(answer.begin(), answer.end());
		return answer;
	}

	std::string toString(int value)
	{
		std::stringstream stream;
		stream << value;
		return stream.str();
	}

	template<typename RandomAccessIterator, typename Equal>
		std::vector<std::string> editDistance(
				RandomAccessIterator aFirst, RandomAccessIterator aLast,
				RandomAccessIterator bFirst, RandomAccessIterator bLast,
				Equal comparator)
	{
		std::vector<RandomAccessIterator> lcs = largestCommonSubsequence(aFirst, aLast, bFirst, bLast, comparator);
		lcs.push_back(bLast);
		std::vector<std::string> answer;
		RandomAccessIterator firstPos = aFirst, secondPos = bFirst;
		int indexAdd = 0;
		for (auto it : lcs)
		{
			while (firstPos != aLast && !comparator(*firstPos, *it))
			{
				if (secondPos != it)
				{
					answer.push_back("CHANGE a[" + toString(firstPos - aFirst + indexAdd) + "] to "
											"b[" + toString(secondPos - bFirst) + "]");
					++secondPos;
				}
				else
				{
					answer.push_back("ERASE a[" + toString(firstPos - aFirst + indexAdd) + "]");
					--indexAdd;
				}
				++firstPos;
			}
			while (secondPos != it)
			{
				answer.push_back("INSERT b[" + toString(secondPos - bFirst) + "] at position " + toString(firstPos - aFirst + indexAdd));
				++indexAdd;
				++secondPos;
			}
			++firstPos;
			++secondPos;
		}
		return answer;
	}

}

#endif // EDITDIST_H
