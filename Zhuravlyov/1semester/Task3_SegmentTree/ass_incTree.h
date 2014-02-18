#ifndef ASSIGN_INCREASE_TREE
#define ASSIGN_INCREASE_TREE

#include "advancedTree.h"

template <class Type, Type neutralSumElement, 
	Type neutralMinElement, Type neutralMaxElement>
class SumMinMaxIncreaseAssignTree
{
public:

	friend struct SumMinMaxIncreaseAssignTreeTestFriend;

	template <class RandomAccessIterator>
	SumMinMaxIncreaseAssignTree(RandomAccessIterator first, RandomAccessIterator last)
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

	void increase(unsigned int left, unsigned int right, const Type& change_value)
	{
		tree.change(left, right, MetaInformation(1, change_value));
	}

	void assign(unsigned int left, unsigned int right, const Type& change_value)
	{
		tree.change(left, right, MetaInformation(0, change_value));
	}

private:

	struct MetaInformation
	{
		bool operation; // 0 - assign, 1- increase
		Type changed;
	
		MetaInformation (bool operation, const Type& change)
		: operation(operation),changed(change) {}
	};

	struct Push 
	{
		void operator()(ReturnType& value_to_change, MetaInformation* changes, unsigned int len)
		{
			if (changes->operation) // increase 
			{
				value_to_change.sum += changes->changed * len;
				value_to_change.max += changes->changed;
				value_to_change.min += changes->changed;
			}
			else //assign
			{
				value_to_change.sum = changes->changed * len;
				value_to_change.max = changes->changed;
				value_to_change.min = changes->changed;
			}
		}
	};
	
	struct Merge
	{
		void operator()(MetaInformation* old_changes, MetaInformation* new_changes)
		{
				if (old_changes->operation) // here is increasing
				{ 
					if (new_changes->operation) //merge with increasing
					{
						old_changes->changed += new_changes->changed; // got increasing again
					}
					else //merge with assign
					{
						old_changes->operation=0;
						old_changes->changed = new_changes -> changed; // got assign now
					}
				} 
				else // here is assigning
				{
					if (new_changes->operation) //merge with increasing
					{
						old_changes->changed += new_changes->changed; // got increased assigning now
					}
					else //merge with assigning
					{
						old_changes->changed = new_changes->changed; // got new assigning now
					}
				}
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