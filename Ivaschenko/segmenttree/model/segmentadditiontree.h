#ifndef SEGMENTADDITIONTREE_H
#define SEGMENTADDITIONTREE_H

#include <cstddef>

#include "model/generalsegmenttree.h"

template<typename DataType> class SegmentAdditionTree
{
	public:
		SegmentAdditionTree(std::size_t size,
							const DataType &negInf, const DataType &posInf, const DataType &zero):
			tree(size, ReturnType(posInf, negInf, zero)) {}

		template<typename ForwardIterator>
		SegmentAdditionTree(ForwardIterator begin, ForwardIterator end,
							const DataType &negInf, const DataType &posInf, const DataType &zero):
			tree(begin, end, ReturnType(posInf, negInf, zero)) {}

		struct ReturnType
		{
			public:
				ReturnType(const DataType &value): min(value), max(value), sum(value) {}
				ReturnType(const DataType &nMin, const DataType &nMax, const DataType &nSum):
					min(nMin), max(nMax), sum(nSum) {}

				DataType min, max, sum;
		};

		ReturnType get(std::size_t left, std::size_t right)
		{
			return tree.get(left, right);
		}

		void update(std::size_t left, std::size_t right, const DataType &value)
		{
			tree.update(left, right, MetaInformation(value));
		}

	private:
		struct Function
		{
			public:
				ReturnType operator () (const ReturnType &a, const ReturnType &b)
				{
					return ReturnType(min(a.min, b.min), max(a.max, b.max), a.sum + b.sum);
				}
		};

		struct MetaInformation
		{
			public:
				MetaInformation(): addValue(0) {}
				MetaInformation(const DataType &nValue): addValue(nValue) {}

				DataType addValue;
		};

		class MetaUpdater
		{
			public:
				void operator () (ReturnType &value, const MetaInformation &info, std::size_t left, std::size_t right)
				{
					value.min += info.addValue;
					value.max += info.addValue;
					value.sum += info.addValue * (right - left + 1);
				}
		};

		class MetaMerger
		{
			public:
				void operator () (MetaInformation &first, const MetaInformation &second, std::size_t, std::size_t)
				{
					first.addValue += second.addValue;
				}
		};

		GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger> tree;
};

#endif // SEGMENTADDITIONTREE_H
