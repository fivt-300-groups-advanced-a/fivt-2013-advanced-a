#ifndef MAXIMALSUMSUBSEGMENT_H
#define MAXIMALSUMSUBSEGMENT_H

#include <gtest/gtest.h>

#include "model/generalsegmenttree.h"

class MaximalSumSubSegmentTree
{
	public:
		template<typename ForwardIterator> MaximalSumSubSegmentTree(ForwardIterator begin, ForwardIterator end)
			: tree(begin, end, ReturnType()) {}

	private:
		struct ReturnType
		{
			public:
				ReturnType(int nValue): sum(nValue), neutral(false)
				{
					sum = nValue;
					if (value < 0)
					{
						maxPrefix = maxSuffix = maxSubseg = nValue;
						//prefPos = , suffPos = 0, segLeft = 0, segRight = 1;
						// TODO: WTF??
					}
					else
					{

					}
				}

				ReturnType(): neutral(true) {}

				int sum, maxPrefix, maxSuffix, maxSubseg;
				int prefPos, suffPos, segLeft, segRight;
				bool neutral;
		};

		struct MetaInformation
		{
			public:
				MetaInformation(): assigned(false) {}
				MetaInformation(int nValue): assigned(true), value(nValue) {}

				bool assigned;
				int value;
		};

		struct Function
		{
			public:
				void operator () (ReturnType &a, const ReturnType &b)
				{
					if (b.neutral) return;
					if (a.neutral || b.maxSubseg > a.maxSubseg)
					{
						a.maxSubseg = b.maxSubseg;
						a.segLeft = b.segLeft;
						a.segRight = b.segRight;
					}
					if (a.maxSuffix + b.maxPrefix > a.maxSubseg)
					{
						a.maxSubseg = a.maxSuffix + b.maxSuffix;
						a.segLeft = a.suffPos; // inclusive suffix
						a.segRight = b.prefPos; // exclusive prefix
					}

					if (a.neutral || b.maxPrefix + a.sum > a.maxPrefix)
					{
						a.maxPrefix = b.maxPrefix + a.sum;
						a.prefPos = b.prefPos; // exclusive prefix
					}

					if (a.neutral || a.maxSuffix + b.sum > a.maxSuffix)
					{
						a.maxSuffix = a.maxSuffix + b.sum;
						a.suffPos = a.suffPos; // Unchanged! (inclusive)
					}
					a.neutral = false;
					a.sum += b.sum;
				}
		};

		struct MetaUpdater
		{
			public:
				void operator () (ReturnType &value, const MetaInformation &info,
								  std::size_t left, std::size_t right)
				{
					value.sum = value.size * info.value;

					if (info.value < 0)
					{
						value.maxPrefix = value.maxSuffix = value.maxSubseg = 0;
						value.prefPos = left, value.suffPos = right + 1;
					}
					else
					{
						value.maxPrefix = value.maxSuffix = value.maxSubseg = value.sum;
						value.prefPos = right + 1, value.suffPos = left;
					}
				}
		};

		struct MetaMerger
		{
			public:
				void operator() (MetaInformation &a, const MetaInformation &b)
				{
					if (b.assigned) a = b;
				}
		};

		GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger> tree;
};

#endif // MAXIMALSUMSUBSEGMENT_H
