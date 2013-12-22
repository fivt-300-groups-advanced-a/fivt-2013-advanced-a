#ifndef SEGMENTASSIGNMENTTREE_H
#define SEGMENTASSIGNMENTTREE_H

#include <functional>
#include <cstddef>

#include "model/generalsegmenttree.h"

// TODO: documentation
template<typename DataType, typename Comparator = std::less<DataType> > class SegmentAssignmentTree
{
	public:
		SegmentAssignmentTree(std::size_t size,
							const DataType &negInf, const DataType &posInf, const DataType &zero,
							Comparator cmp = std::less<DataType>()):
			tree(size, ReturnType(posInf, negInf, zero, Function(cmp))) {}

		template<typename ForwardIterator>
		SegmentAssignmentTree(ForwardIterator begin, ForwardIterator end,
							const DataType &negInf, const DataType &posInf, const DataType &zero,
							Comparator cmp = std::less<DataType>()):
			tree(begin, end, ReturnType(posInf, negInf, zero), Function(cmp)) {}

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
				Function(const Comparator nCmp): cmp(nCmp) {}

				ReturnType operator () (const ReturnType &a, const ReturnType &b)
				{
					return ReturnType(std::min(a.min, b.min, cmp), std::max(a.max, b.max, cmp), a.sum + b.sum);
				}

			private:
				Comparator cmp;
		};

		struct MetaInformation
		{
			public:
				MetaInformation(): assignValue(0), assigned(false) {}
				MetaInformation(const DataType &nValue): assignValue(nValue), assigned(true) {}

				DataType assignValue;
				bool assigned;
		};

		class MetaUpdater
		{
			public:
				void operator () (ReturnType &value, const MetaInformation &info, std::size_t left, std::size_t right)
				{
					if (!info.assigned) return;
					value.min = info.assignValue;
					value.max = info.assignValue;
					value.sum = info.assignValue * (right - left + 1);
				}
		};

		class MetaMerger
		{
			public:
				void operator () (MetaInformation &first, const MetaInformation &second, std::size_t, std::size_t)
				{
					if (second.assigned) first = second;
				}
		};

		GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger> tree;
};

#endif // SEGMENTASSIGNMENTTREE_H
