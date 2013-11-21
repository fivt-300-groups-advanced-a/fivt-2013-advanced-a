#ifndef UNIONHEAP
#define UNIONHEAP

#include <vector>
#include "BinTree.h"
#include "friend_functions.h"

template < class Type,class Comparator >
class BinominalyHeap{
public:

typedef ValueReference<BinominalyTree<Type,Comparator>>* pointer;

private:

	typedef BinominalyTree<Type,Comparator> BinT;

	template < class Type,class Comparator > friend class BinominalyTree;

	std::vector<BinT*> trees;

	void add_by_pointer(BinT*& pointer)
	{
		if (trees.size()==0) trees.push_back(pointer);
		else if (trees[0]==NULL) trees[0]=pointer;
		else {
			BinominalyHeap<Type,Comparator> fictive_heap;
			fictive_heap.trees.push_back(pointer);
			merge(fictive_heap);
		}
	}

	int find_max_ind()
	{
		int max_ind=0;
		while (!trees[max_ind]) max_ind++;
		Comparator cmp;
		for (unsigned int i=max_ind+1;i<trees.size();i++)
			if (trees[i])
				if (cmp(trees[i]->top(),trees[max_ind]->top())) max_ind=i;
		return max_ind;
	}

	void erase_by_maxind(const int &max_ind)
	{
		BinominalyHeap temporary;
		temporary.trees=trees[max_ind]->childs;
		delete trees[max_ind];
		trees[max_ind]=NULL;
		while (trees.size()>0 && trees[trees.size()-1]==NULL) 
			trees.pop_back();
		for (unsigned int i=0;i<temporary.trees.size();i++)
			temporary.trees[i]->parent=NULL;
		merge(temporary);
	}

public:
	BinominalyHeap() {}

	bool empty()
	{
		return !trees.size();
	}

	Type getM()
	{
		if (!trees.empty()) {
			return trees[find_max_ind()]->top();
		}
	}
	
	Type extract()
	{
		if (!trees.empty()) {
			int max_ind=find_max_ind();
			Type result=trees[max_ind]->top();
			erase_by_maxind(max_ind);
			return result;
		}
	}

	pointer insert(const Type &element)
	{
		BinT* back_pointer=new BinT(element);
		add_by_pointer(back_pointer);
		pointer unique_pointer=new ValueReference<BinT>;
		unique_pointer->node=back_pointer;
		back_pointer->backward_pointer=unique_pointer;
		return unique_pointer;
	}

	void merge(BinominalyHeap<Type,Comparator>& assigned) {
		int q=std::max(trees.size(),assigned.trees.size());
		trees.resize(q);
		assigned.trees.resize(q);
		BinT* carry=NULL;
		for (int i=0;i<q;i++)
			getresult(trees[i],assigned.trees[i],carry);
		if (carry) trees.push_back(carry);
		assigned.trees.resize(0);
	}

	void delete_element(pointer& pointer) 
	{
		sift_to_top(pointer->node);
		int maxind;
		for (unsigned int i=0;i<trees.size();i++)
			if (pointer->node==trees[i]) maxind=i;
		erase_by_maxind(maxind);
		pointer=NULL;
	}

	void change_key_to(pointer& pointer,Type newValue)
	{
		Comparator cmp;
		if (cmp(newValue,pointer->node->top()))
		{
			pointer->node->info=newValue;
			sift_up(pointer->node);
		} else {
			delete_element(pointer);
			pointer=insert(newValue);
		}
	}

	void change_key_on(pointer& pointer,Type delta)
	{
		change_key_to(pointer,(pointer->node)->top()+delta);
	}

	bool check_invariants()
	{
		bool f=1;
		for (unsigned int i=0;i<trees.size();i++)
			if (!check_invariants_for_trees(trees[i])) f=0;
			else {
				int quantity_of_elements=quantity(trees[i]);
				if (quantity_of_elements!=1<<i && quantity_of_elements!=0) 
					f=0;
			}
		return f;
	}

	Type get_by_ptr(pointer pointer)
	{
		return (pointer->node)->top();
	}

	~BinominalyHeap(){
		for (unsigned int i=0;i<trees.size();i++)
			clear_tree(trees[i]);
		trees.resize(0);
	}
};

#endif