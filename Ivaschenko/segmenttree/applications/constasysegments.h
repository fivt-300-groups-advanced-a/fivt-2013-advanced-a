#ifndef CONSTASYSEGMENTS_H
#define CONSTASYSEGMENTS_H

#include "model/generalsegmenttree.h"

template <typename DataType> class ConstansySegmentsTree
{
	public:
		template <typename ForwardIterator> ConstansySegmentsTree(ForwardIterator begin, ForwardIterator end):
			tree(begin, end, ReturnType()) {}

		ConstansySegmentsTree(std::size_t n): tree(n, ReturnType()) {}

		std::size_t countConstansySegments(std::size_t left, std::size_t right)
		{
			return tree.get(left, right).segCount;
		}

	private:
		struct ReturnType
		{
			public:
				ReturnType(): segCount(0), identity(true) {}
				ReturnType(const DataType &value): leftValue(value), rightValue(value), segCount(1) {}
				ReturnType(const DataType &nLeft, const DataType &nRight, std::size_t nCount):
					leftValue(nLeft), rightValue(nRight), segCount(nCount) {}

				DataType leftValue, rightValue;
				std::size_t segCount;
				bool identity;
		};

		struct MetaInformation
		{
			public:
				MetaInformation(): assigned(false) {}
				MetaInformation(const DataType &nValue): value(nValue), assigned(true) {}

				DataType value;
				bool assigned;
		};

		struct Function
		{
			public:
				ReturnType operator () (const ReturnType &a, const ReturnType &b) const
				{
					std::size_t nSegments = a.segCount + b.segCount - (*a.rightValue == *b.leftValue ? 1 : 0);
					return ReturnType(a.leftValue, b.rightValue, nSegments);
				}
		};

		struct MetaUpdater
		{
			public:
				void operator () (ReturnType &value, const MetaInformation &info) const
				{
					if (!info.assigned) return;
					value.leftValue = value.rightValue = info.value;
					value.segCount = 1;
				}
		};

		struct MetaMerger
		{
			public:
				void operator () (MetaInformation &a, const MetaInformation &b) const
				{
					if (b.assigned) a = b;
				}
		};

		GeneralSegmentTree<ReturnType, MetaInformation, Function, MetaUpdater, MetaMerger> tree;
};

#endif // CONSTASYSEGMENTS_H
