#ifndef FAKETREE
#define FAKETREE

//Fake segment tree for inner state testing - not ready yet

#include "advancedTree.h"
#include <vector>

struct fakeTree
{
	int metaQuantity, number_of_pushes, number_of_merges, number_of_unions;
	bool correct_push, correct_merge, correct_union, correct_build;

	template <class Iterator>
	fakeTree(Iterator begin, Iterator end): tree(begin, end) 
	{
		metaQuantity = number_of_pushes	= number_of_merges = 0;
		correct_push = correct_merge = correct_union = correct_build = true;
		for (unsigned int i=1; i <= tree.tree.size(); i++)
			if (i != tree.tree[i].first.value) 
				correct_build = false;
	}

	struct ReturnType
	{
		int value;
		ReturnType(): value(-1)
		{}
		ReturnType(int number): value(number)
		{}
	};

	struct MetaInformation
	{
		MetaInformation() 
		{
			metaQuantity++;
		}

		~MetaInformation() {
			metaQuantity--;
		}
	};

	struct Push 
	{
		void operator()(ReturnType& issue, MetaInformation* changes, unsigned int len)
		{
			number_of_pushes++;
			if (!changes) 
				fakeTree::correct_push = false;
		}
	};

	struct Merge
	{
		void operator()(MetaInformation* old_changes, MetaInformation* new_changes)
		{
			fakeTree::number_of_merges++;
			if (!old_changes || !new_changes)
				fakeTree::correct_merge = false;
		}
	};

	struct Unite
	{
		ReturnType operator()(const ReturnType& first, const ReturnType& second)
		{
			number_of_unions++;
			if (first.value == -1 || second.value -1) 
				return ReturnType(-1);
			if (first.value / 2 != second.value / 2) 
				correct_union = false;
			return ReturnType(first.value/2);
		}
	};

	void get(int left, int right)
	{
		tree.get(left, right);
	}

	void change(int left, int right)
	{
		tree.change(left, right, MetaInformation());
	}

	AdvancedSegmentTree<ReturnType, MetaInformation, Push, Merge, Unite > tree;
};


#endif