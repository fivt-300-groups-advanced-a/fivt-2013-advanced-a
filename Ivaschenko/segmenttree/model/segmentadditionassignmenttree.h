#ifndef SEGMENTADDITIONASSIGNMENTTREE_H
#define SEGMENTADDITIONASSIGNMENTTREE_H

#include <functional>
#include <cstddef>

#include "model/generalsegmenttree.h"

// TODO: documentation
template<typename DataType, typename Comparator = std::less<DataType> > class SegmentAdditionAssignmentTree
{
	public:
		SegmentAdditionAssignmentTree(std::size_t size,
							const DataType &negInf, const DataType &posInf, const DataType &zero,
							Comparator cmp = std::less<DataType>()):
			tree(size, ReturnType(posInf, negInf, zero, Function(cmp))) {}

		template<typename ForwardIterator>
		SegmentAdditionAssignmentTree(ForwardIterator begin, ForwardIterator end,
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

		void assign(std::size_t left, std::size_t right, const DataType &value)
		{
			tree.update(left, right, MetaInformation(1, value));
		}

		void add(std::size_t left, std::size_t right, const DataType &value)
		{
			tree.update(left, right, MetaInformation(0, value));
		}

	private:
		struct Function
		{
			public:
				Function(const Comparator nCmp): cmp(nCmp) {}

				ReturnType operator () (const ReturnType &a, const ReturnType &b) const
				{
					return ReturnType(std::min(a.min, b.min, cmp), std::max(a.max, b.max, cmp), a.sum + b.sum);
				}

			private:
				Comparator cmp;
		};

		struct MetaInformation
		{
			public:
				MetaInformation(): addValue(0), assignValue(0), assigned(false) {}
				MetaInformation(const int type, const DataType &nValue)
				{
					assigned = type;
					addValue = type ? 0 : nValue;
					assignValue = type ? nValue : 0;
				}

				DataType addValue, assignValue;
				bool assigned;
		};

		class MetaUpdater
		{
			public:
				void operator () (ReturnType &value, const MetaInformation &info, std::size_t left, std::size_t right) const
				{
					if (info.assigned)
					{
						value.min = info.addValue + info.assignValue;
						value.max = info.addValue + info.assignValue;
						value.sum = (info.addValue + info.addValue) * (right - left + 1);
					}
					else
					{
						value.min += info.addValue;
						value.max += info.addValue;
						value.sum += info.addValue * (right - left + 1);
					}
				}
		};

		class MetaMerger
		{
			public:
				void operator () (MetaInformation &first, const MetaInformation &second, std::size_t, std::size_t) const
				{
					if (second.assigned) first = second;
					else first.addValue += second.addValue;
				}
		};

		GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger> tree;
};


#endif // SEGMENTADDITIONASSIGNMENTTREE_H
