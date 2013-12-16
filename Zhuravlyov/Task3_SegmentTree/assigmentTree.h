#ifndef ASSIGMENT_TREE
#define ASSIGMENT_TREE

#include "advancedTree.h"

template <class Type, Type neutralSumElement, 
	Type neutralMinElement, Type neutralMaxElement>
class SumMinMaxAssignTree
{
public:

	friend struct SumMinMaxAssignTreeTestFriend;

	template <class RandomAccessIterator>
	SumMinMaxAssignTree(RandomAccessIterator first,	RandomAccessIterator last)
		:   tree(first, last)
	{}

	struct ReturnType
	{
		ReturnType(): min(neutralMinElement), max(neutralMaxElement), sum(neutralSumElement)
		{}
		ReturnType(const Type& element): min(element), max(element), sum(element)
		{}
		ReturnType(const Type& sum, const Type& min, const Type& max)
		: sum(sum), min(min), max(max)
		{}
		Type sum, min, max;
	};

	ReturnType get(unsigned int left,unsigned int right)
	{
		return tree.get(left, right);
	}

	void assign(unsigned int left, unsigned int right, const Type& assign_value)
	{
		tree.change(left, right, MetaInformation(assign_value));
	}

private:

	struct MetaInformation
	{
		Type assigned;
		MetaInformation(const Type& to_assign): assigned(to_assign) 
		{}
	};

	struct Push 
	{
		void operator()(ReturnType& value_to_change, MetaInformation* changes, unsigned int len)
		{
			value_to_change.sum = changes->assigned * len;
			value_to_change.max = changes->assigned;
			value_to_change.min = changes->assigned;
		}
	};
	
	struct Merge
	{
		void operator()(MetaInformation* old_changes, MetaInformation* new_changes)
		{
			old_changes->assigned = new_changes->assigned;
		}
	};

	struct Unite
	{
		ReturnType operator()(const ReturnType& first, const ReturnType& second)
		{
			return ReturnType( first.sum + second.sum, std::min(first.min, second.min),
								std::max(first.max, second.max) );
		}
	};

	AdvancedSegmentTree< ReturnType, MetaInformation, Push, Merge, Unite > tree;
};

#endif