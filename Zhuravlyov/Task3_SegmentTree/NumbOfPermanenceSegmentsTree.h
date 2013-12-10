#ifndef PERFOMANCE_SEGMENTS_TREE
#define PERFOMANCE_SEGMENTS_TREE

#include "advancedTree.h"

template <class Type>
class NumberOfPermanenceSegmentsTree
{
public:

	template <class RandomAccessIterator>
	NumberOfPermanenceSegmentsTree(RandomAccessIterator first, RandomAccessIterator last)
		:   tree(first, last)
	{}

	struct ReturnType{
		ReturnType(): neutral(true) 
		{}
		ReturnType(Type element): neutral(false), number(1), left(element), right(element) 
		{}
		ReturnType(Type left, Type right, unsigned int number)
			: neutral(false), number(number), left(left), right(right) 
		{}

		unsigned int number;
		bool neutral;
		Type left, right;
	};

	unsigned int getNumber(unsigned int left, unsigned int right)
	{
		return tree.get(left, right).number;
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
				value_to_change.left += changes->changed;
				value_to_change.right += changes->changed;
			}
			else //assign
			{
				value_to_change.left = value_to_change.right = changes->changed;
				value_to_change.number = 1;
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
			if (first.neutral) 
				return second;
			else if (second.neutral)
				return first;
			else if (first.right == second.left) 
				return ReturnType(first.left, second.right, first.number + second.number - 1);
			else 
				return ReturnType(first.left, second.right, first.number + second.number);
		}
	};


	AdvancedSegmentTree< ReturnType, MetaInformation, Push, Merge, Unite > tree;
};

#endif