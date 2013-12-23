#ifndef MAXIMALSUMSUBSEGMENT_H
#define MAXIMALSUMSUBSEGMENT_H

#include <utility>

#include "model/generalsegmenttree.h"

template<typename DataType, typename Comparator = std::less<DataType> > class MaximalSumSubSegmentTree
{
	private:
		struct ReturnType;

	public:
		template<typename ForwardIterator> MaximalSumSubSegmentTree(ForwardIterator begin, ForwardIterator end,
																	const DataType &nIdentity = DataType(),
																	Comparator nCmp = Comparator()):
			cmp(nCmp), identity(nIdentity),
			tree(1, ReturnType(), Function(cmp), MetaUpdater(cmp, nIdentity)) /* dirty hack */
		{
			std::vector<ReturnType> data;
			for (std::size_t i = 0; begin != end; ++begin, ++i)
			{
				ReturnType cur;
				if (!cmp(*begin, identity))
				{
					cur.maxPrefix = cur.maxSuffix = cur.maxSubseg = *begin;
					cur.prefPos = i + 1, cur.suffPos = i;
					cur.segLeft = i, cur.segRight = i + 1;
				}
				else
				{
					cur.maxPrefix = cur.maxSuffix = cur.maxSubseg = identity;
					cur.prefPos = i, cur.suffPos = i + 1;
					cur.segLeft = i + 1, cur.segRight = i;
				}
				cur.sum = *begin;
				cur.neutral = false;
				data.push_back(cur);
			}
			tree = GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger>
					(data.begin(), data.end(), ReturnType(), Function(cmp), MetaUpdater(cmp, nIdentity));
		}

		MaximalSumSubSegmentTree(std::size_t n, const DataType &value = DataType(),
								 const DataType &nIdentity = DataType(),
								 Comparator nCmp = Comparator()):
			cmp(nCmp), identity(nIdentity),
			tree(1, ReturnType(), Function(cmp), MetaUpdater(cmp, nIdentity)) /* dirty hack */
		{
			std::vector<ReturnType> data;
			for (std::size_t i = 0; i < n; ++i)
			{
				ReturnType cur;
				cur.maxPrefix = cur.maxSuffix = cur.maxSubseg = identity;
				cur.prefPos = i, cur.suffPos = i + 1;
				cur.segLeft = i + 1, cur.segRight = i;
				cur.neutral = false;
				data.push_back(cur);
			}
			tree = GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger>
					(data.begin(), data.end(), ReturnType(), Function(cmp), MetaUpdater(cmp, nIdentity));
		}

		void update(std::size_t left, std::size_t right, const DataType &value)
		{
			tree.update(left, right, MetaInformation(value));
		}

		struct Segment
		{
			public:
				std::size_t left, right;
				DataType sum;

				Segment(const ReturnType &ans): left(ans.segLeft), right(ans.segRight), sum(ans.maxSubseg) {}
		};

		Segment maximalSubsegment(std::size_t left, std::size_t right)
		{
			return Segment(tree.get(left, right));
		}

	private:
		struct ReturnType
		{
			public:
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
				Function(Comparator nCmp): cmp(nCmp) {}

				ReturnType operator () (const ReturnType &a, const ReturnType &b)
				{
					ReturnType result = a;
					result.maxSuffix = b.maxSuffix;
					result.suffPos = b.suffPos;

					if (b.neutral) return result;
					if (result.neutral || cmp(result.maxSubseg, b.maxSubseg))
					{
						result.maxSubseg = b.maxSubseg;
						result.segLeft = b.segLeft;
						result.segRight = b.segRight;
					}
					if (cmp(result.maxSubseg, a.maxSuffix + b.maxPrefix))
					{
						result.maxSubseg = a.maxSuffix + b.maxPrefix;
						result.segLeft = a.suffPos; // inclusive suffix
						result.segRight = b.prefPos; // exclusive prefix
					}

					if (result.neutral || cmp(result.maxPrefix, a.sum + b.maxPrefix))
					{
						result.maxPrefix = a.sum + b.maxPrefix;
						result.prefPos = b.prefPos; // exclusive prefix
					}

					if (result.neutral || cmp(result.maxSuffix, a.maxSuffix + b.sum))
					{
						result.maxSuffix = a.maxSuffix + b.sum;
						result.suffPos = a.suffPos; // Unchanged! (inclusive)
					}
					result.neutral = false;
					result.sum = (result.neutral ? 0 : result.sum) + b.sum;
					return result;
				}

			private:
				Comparator cmp;
		};

		struct MetaUpdater
		{
			public:
				MetaUpdater(Comparator nCmp, const DataType &nIdentity): cmp(nCmp), identity(nIdentity) {}

				void operator () (ReturnType &value, const MetaInformation &info,
								  std::size_t left, std::size_t right)
				{
					if (!info.assigned) return;
					value.sum = (right - left + 1) * info.value;

					if (cmp(info.value, identity))
					{
						value.maxPrefix = value.maxSuffix = value.maxSubseg = identity;
						value.prefPos = left, value.suffPos = right + 1;
						value.segLeft = right + 1, value.segRight = left;
					}
					else
					{
						value.maxPrefix = value.maxSuffix = value.maxSubseg = value.sum;
						value.prefPos = right + 1, value.suffPos = left;
						value.segLeft = left, value.segRight = right + 1;
					}
				}

			private:
				Comparator cmp;
				DataType identity;
		};

		struct MetaMerger
		{
			public:
				void operator() (MetaInformation &a, const MetaInformation &b, std::size_t, std::size_t)
				{
					if (b.assigned) a = b;
				}
		};

		Comparator cmp;
		DataType identity;
		GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger> tree;
};

#endif // MAXIMALSUMSUBSEGMENT_H
