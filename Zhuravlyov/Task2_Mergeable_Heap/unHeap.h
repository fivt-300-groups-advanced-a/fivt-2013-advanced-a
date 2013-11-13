#ifndef UNIONHEAP
#define UNIONHEAP

#include <vector>
#include "BinTree.h"
#include "friend_functions.h"

template < class Type,class Comparator >
class BinominalyHeap{

typedef BinominalyTree<Type,Comparator> BinT;

template < class Type,class Comparator > friend class BinominalyTree;

private:
	std::vector<BinT*> trees;

	void refresh_tree_roots()
	{
		for (unsigned int i=0;i<trees.size();i++)
		if (trees[i])
			if (trees[i]->parent) trees[i]=trees[i]->parent;
	}

	void delete_without_freeing(BinT* pointer)
	{
		sift_to_top(pointer);
		refresh_tree_roots();
		BinominalyHeap temporary;
		temporary.trees=pointer->childs;
		for (unsigned int i=0;i<trees.size();i++)
			if (trees[i]==pointer) trees[i]=NULL;
		while (trees.size()>0 && !trees[trees.size()-1]) 
			trees.pop_back();
		for (unsigned int i=0;i<temporary.trees.size();i++)
			temporary.trees[i]->parent=NULL;
		merge(temporary);
	}

	void add_by_pointer(BinT*& pointer)
	{
		BinominalyHeap<Type,Comparator> fictive_heap;
		fictive_heap.trees.push_back(pointer);
		merge(fictive_heap);
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

public:
	typedef BinT* pointer;

	BinominalyHeap()
	{
		trees.resize(0);
	}

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
			BinominalyHeap temporary;
			temporary.trees=trees[max_ind]->childs;
			delete trees[max_ind];
			trees[max_ind]=NULL;
			while (trees.size()>0 && trees[trees.size()-1]==NULL) 
				trees.pop_back();
			for (unsigned int i=0;i<temporary.trees.size();i++)
				temporary.trees[i]->parent=NULL;
			merge(temporary);
			return result;
		}
	}

	BinT* insert(const Type &element)
	{
		BinT* back_pointer=new BinT(element);
		add_by_pointer(back_pointer);
		return back_pointer;
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

	void delete_element(BinT*& pointer) 
	{
		delete_without_freeing(pointer);
		delete pointer;
		pointer=NULL;
	}

	void change_key_to(BinT* pointer,Type newValue)
	{
		Comparator cmp;
		if (cmp(newValue,pointer->top()))
		{
			pointer->info=newValue;
			sift_up(pointer);
			refresh_tree_roots();
		} else {
			delete_without_freeing(pointer);
			pointer->info=newValue;
			pointer->parent=NULL;
			pointer->childs.resize(0);
			add_by_pointer(pointer);
		}
	}

	void change_key_on(BinT* pointer,Type delta)
	{
		change_key_to(pointer,pointer->top()+delta);
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

	Type get_by_ptr(BinT* pointer)
	{
		return pointer->top();
	}

	~BinominalyHeap(){
		for (unsigned int i=0;i<trees.size();i++)
			clear_tree(trees[i]);
		trees.resize(0);
	}
};

#endif