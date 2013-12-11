#ifndef MAXSUBSEGMENTTREE
#define MAXSUBSEGMENTTREE

#include "advancedTree.h"

template <class Type, Type neutralSumElement, Type neutralMaxElement>
class MaxSubsegmentTree
{
public:

	template <class RandomAccessIterator>
	MaxSubsegmentTree(RandomAccessIterator first, RandomAccessIterator last)
		:   tree(first, last)
	{}

	struct ReturnType
	{
		Type max_prefix, max_suffix, max_subsum, sum;

		ReturnType(): max_prefix(neutralMaxElement), max_suffix(neutralMaxElement),
			max_subsum(neutralMaxElement), sum(neutralSumElement)
		{}
		ReturnType(Type element): max_prefix(element), max_suffix(element),
			max_subsum(element), sum(element)
		{}
		ReturnType(Type max_prefix, Type max_suffix, Type max_subsum, Type sum)
		: max_prefix(max_prefix), max_suffix(max_suffix), max_subsum(max_subsum), sum(sum)
		{}
	};

	Type getMaxSubSum(unsigned int left, unsigned int right)
	{
		return tree.get(left,right).max_subsum;
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
			if (changes->assigned > neutralSumElement) {
				value_to_change.max_prefix = value_to_change.max_suffix
					= value_to_change.max_subsum = changes->assigned * len;
			} 
			else 
			{
				value_to_change.max_prefix = value_to_change.max_suffix
					= value_to_change.max_subsum = changes->assigned;
			}
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
		Type safe_sum(const Type& first, const Type& second)
		{
			if (first==neutralMaxElement || second==neutralMaxElement)
				return neutralMaxElement;
			else 
				return first + second;
		}

		ReturnType operator()(const ReturnType& first, const ReturnType& second)
		{
			return ReturnType( 
				std::max( first.max_prefix, safe_sum(first.sum, second.max_prefix) ),
				std::max( second.max_suffix, safe_sum( second.sum, first.max_suffix) ),
				std::max( std::max(first.max_subsum, second.max_subsum), safe_sum(first.max_suffix, second.max_prefix)),
				first.sum + second.sum );
		}
	};

	AdvancedSegmentTree<ReturnType, MetaInformation, Push, Merge, Unite> tree;
};

#endif