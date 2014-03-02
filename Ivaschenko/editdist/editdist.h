#ifndef EDITDIST_H
#define EDITDIST_H

#include <vector>
#include <algorithm>
#include <functional>

#include <cassert>

namespace editdist
{
	template<typename RandomAccessIterator> class EditAction
	{
		public:
			typedef enum
			{
				ACTION_INSERT,
				ACTION_ERASE,
				ACTION_CHANGE
			} ActionType;

			EditAction(ActionType nType, std::size_t idx, RandomAccessIterator nValue):
				type(nType), index(idx), value(nValue) {}

			EditAction(ActionType nType, std::size_t idx):
				type(nType), index(idx)
			{
				assert(type == ACTION_ERASE);
			}

			ActionType getType() const
			{
				return type;
			}

			std::size_t getIndex() const
			{
				return index;
			}

			RandomAccessIterator getAssignedValue() const
			{
				assert(type != ACTION_ERASE);
				return value;
			}

			bool operator < (const EditAction<RandomAccessIterator> &act) const
			{
				return index > act.index;
			}

		private:
			ActionType type;
			std::size_t index;
			RandomAccessIterator value;
	};

	template<typename RandomAccessIterator> class IteratorWrapper
	{
		public:
			IteratorWrapper(RandomAccessIterator it, bool revFlag): start(it), reverse(revFlag) {}

			IteratorWrapper<RandomAccessIterator> operator + (int x) const
			{
				return reverse ? IteratorWrapper(start - x, reverse) : IteratorWrapper(start + x, reverse);
			}

			RandomAccessIterator operator * () const
			{
				return start;
			}

		private:
			RandomAccessIterator start;
			bool reverse;
	};

	template<typename RAIteratorWrapper, typename Equal>
		std::vector<std::size_t> editDistPrefix(RAIteratorWrapper aFirst, std::size_t n,
												RAIteratorWrapper bFirst, std::size_t m,
												Equal comparator)
	{
		std::size_t *dp[2];
		for (int i = 0; i < 2; ++i)
		{
			dp[i] = new std::size_t[m + 1];
			std::fill(dp[i], dp[i] + m + 1, m + n);
		}
		for (int i = 0; i <= m; ++i) dp[0][i] = i;
		for (std::size_t i = 0; i < n; ++i)
		{
			dp[1][0] = i + 1;
			for (std::size_t j = 1; j <= m; ++j)
			{
				dp[1][j] = std::min(dp[1][j], dp[0][j] + 1);
				dp[1][j] = std::min(dp[1][j], dp[1][j - 1] + 1);
				if (comparator(**(aFirst + i), **(bFirst + (j - 1))))
					dp[1][j] = std::min(dp[1][j], dp[0][j - 1]);
				else
					dp[1][j] = std::min(dp[1][j], dp[0][j - 1] + 1);
			}
			std::swap(dp[0], dp[1]);
			std::fill(dp[1], dp[1] + m + 1, m + n);
		}
		std::vector<std::size_t> answer(dp[0], dp[0] + m + 1);
		for (int i = 0; i < 2; ++i) delete [] dp[i];
		return answer;
	}

	template<typename RandomAccessIterator, typename Equal>
		std::vector< EditAction<RandomAccessIterator> > editDist(
				RandomAccessIterator aFirst, RandomAccessIterator aLast,
				RandomAccessIterator bFirst, RandomAccessIterator bLast,
				Equal comparator, std::size_t offset = 0)
	{
		typedef EditAction<RandomAccessIterator> Action;
		std::size_t n = aLast - aFirst, m = bLast - bFirst;
		if (n == 1)
		{
			auto pos = std::find(bFirst, bLast, *aFirst);
			std::vector<Action> answer;
			if (pos == bLast && bFirst == bLast) answer.push_back(Action(Action::ACTION_ERASE, offset));
			else if (pos == bLast)
			{
				answer.push_back(Action(Action::ACTION_CHANGE, offset, bFirst));
				pos = bFirst;
			}

			for (std::size_t cur = 0; bFirst != bLast; ++bFirst)
				if (bFirst != pos) answer.push_back(Action(Action::ACTION_INSERT, offset + cur, bFirst));
				else ++cur;
			std::reverse(answer.begin(), answer.end());
			return answer;
		}
		std::size_t firstHalf = n / 2;
		std::size_t bestPartition = 0;
		{
			std::vector<std::size_t> first =  editDistPrefix(IteratorWrapper<RandomAccessIterator>(aFirst, false), firstHalf,
															 IteratorWrapper<RandomAccessIterator>(bFirst, false), m, comparator);
			std::vector<std::size_t> second = editDistPrefix(IteratorWrapper<RandomAccessIterator>(aLast - 1, true), n - firstHalf,
															 IteratorWrapper<RandomAccessIterator>(bLast - 1, true), m, comparator);

			for (std::size_t i = 0; i <= m; ++i)
				if (first[i] + second[m - i] < first[bestPartition] + second[m - bestPartition])
					bestPartition = i;
		}
		std::vector<Action> left =
				editDist(aFirst, aFirst + firstHalf,
										 bFirst, bFirst + bestPartition,
										 comparator, offset);
		std::vector<Action> right =
				editDist(aFirst + firstHalf, aLast,
										 bFirst + bestPartition, bLast,
										 comparator, offset + firstHalf);
		std::vector<Action> answer;
		std::merge(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(answer));
		return answer;
	}

	template<typename RandomAccessIterator, typename Equal>
		std::size_t dummyEditDist(RandomAccessIterator aFirst, RandomAccessIterator aLast,
							 RandomAccessIterator bFirst, RandomAccessIterator bLast,
							 Equal comparator)
	{
		std::size_t n = aLast - aFirst, m = bLast - bFirst;
		return editDistPrefix(IteratorWrapper<RandomAccessIterator>(aFirst, false), n,
							  IteratorWrapper<RandomAccessIterator>(bFirst, false), m,
							  comparator)[m];
	}
}

#endif // EDITDIST_H
