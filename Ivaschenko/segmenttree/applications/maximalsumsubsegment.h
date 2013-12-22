#ifndef MAXIMALSUMSUBSEGMENT_H
#define MAXIMALSUMSUBSEGMENT_H

#include <utility>

#include "model/generalsegmenttree.h"

template<typename DataType, typename Comparator = std::less<DataType> > class MaximalSumSubSegmentTree
{
	public:
		template<typename ForwardIterator> MaximalSumSubSegmentTree(ForwardIterator begin, ForwardIterator end,
																	const DataType &nIdentity = DataType(),
																	Comparator nCmp = Comparator()):
			cmp(nCmp), identity(nIdentity)
		{
			std::vector< std::pair<DataType, int> > data;
			for (size_t i = 0; begin != end; ++begin, ++i)
				data.push_back(std::make_pair(*begin, i));
			tree = GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger>
					(data.begin(), data.end(), ReturnType());
		}

		template<typename ForwardIterator> MaximalSumSubSegmentTree(std::size_t n, const DataType &value = DataType(),
																	const DataType &nIdentity = DataType(),
																	Comparator nCmp = Comparator()):
			cmp(nCmp), identity(nIdentity)
		{
			std::vector< std::pair<DataType, int> > data(n, value);
			tree = GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger>
					(data.begin(), data.end(), ReturnType());
		}

	private:
		struct ReturnType
		{
			public:
				ReturnType(const std::pair<DataType, int> &p): sum(p.first), neutral(false)
				{
					if (!cmp(sum, identity))
					{
						maxPrefix = maxSuffix = maxSubseg = sum;
						prefPos = p.second + 1, suffPos = p.second;
						segLeft = p.second + 1, segRight = p.second;
					}
					else
					{
						maxPrefix = maxSuffix = maxSubseg = 0;
						prefPos = p.second, suffPos = p.second + 1;
						segLeft = p.second, segRight = p.second + 1;
					}
				}

				ReturnType(): neutral(true) {}

				DataType sum, maxPrefix, maxSuffix, maxSubseg;
				int prefPos, suffPos, segLeft, segRight;
				bool neutral;
		};

		struct MetaInformation
		{
			public:
				MetaInformation(): assigned(false) {}
				MetaInformation(const DataType &nValue): assigned(true), value(nValue) {}

				bool assigned;
				DataType value;
		};

		struct Function
		{
			public:
				void operator () (ReturnType &a, const ReturnType &b)
				{
					if (b.neutral) return;
					if (a.neutral || cmp(a.maxSubseg, b.maxSubseg))
					{
						a.maxSubseg = b.maxSubseg;
						a.segLeft = b.segLeft;
						a.segRight = b.segRight;
					}
					if (cmp(a.maxSubseg, a.maxSuffix + b.maxPrefix))
					{
						a.maxSubseg = a.maxSuffix + b.maxSuffix;
						a.segLeft = a.suffPos; // inclusive suffix
						a.segRight = b.prefPos; // exclusive prefix
					}

					if (a.neutral || cmp(a.maxPrefix, b.maxPrefix + a.sum))
					{
						a.maxPrefix = b.maxPrefix + a.sum;
						a.prefPos = b.prefPos; // exclusive prefix
					}

					if (a.neutral || cmp(a.maxSuffix, a.maxSuffix))
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
					value.sum = (right - left + 1) * info.value;

					if (cmp(info.value, identity))
					{
						value.maxPrefix = value.maxSuffix = value.maxSubseg = DataType(0);
						value.prefPos = left, value.suffPos = right + 1;
						value.segLeft = left, value.segRight = right + 1;
					}
					else
					{
						value.maxPrefix = value.maxSuffix = value.maxSubseg = value.sum;
						value.prefPos = right + 1, value.suffPos = left;
						value.segLeft = -1, value.segRight = -1;
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

		Comparator cmp;
		DataType identity;
		GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger> tree;
};

#endif // MAXIMALSUMSUBSEGMENT_H
