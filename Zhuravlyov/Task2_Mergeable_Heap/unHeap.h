#ifndef UNIONHEAP
#define UNIONHEAP

#include <vector>
#include "BinTree.h"


template < class Type,class Comparator >
class BinominalyHeap{
template < class Type,class Comparator > friend class BinominalyTree;
typedef BinominalyTree<Type,Comparator> BinT;
private:
	std::vector<BinT*> trees;
public:
	static std::vector<BinT*> adressa;
//	template <class Type,class Comparator >	friend void sift_to_top(BinT*&); 
//	template <class Type,class Comparator >	friend void sift_up(BinT*&);
//	template <class Type,class Comparator >	friend void clear_tree(BinT*); 
//	template <class Type,class Comparator >	friend int quantity(BinT*);
//	template <class Type,class Comparator >	friend bool check_invariants_for_trees(BinT*);

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
		Comparator cmp;
		if (!trees.empty()) {
			Type max=trees[0]->top();
			for (int i=1;i<trees.size();i++)
				if (cmp(trees[i]->top(),max)) max=trees[i]->top();
			return max;
		}
	}
	
	Type extract()
	{
		if (!trees.empty()) {
			int max_ind=0;
			while (!trees[max_ind]) max_ind++;
			Comparator cmp;
			for (unsigned int i=max_ind+1;i<trees.size();i++)
				if (trees[i])
					if (cmp(trees[i]->top(),trees[max_ind]->top())) max_ind=i;
			Type result=trees[max_ind]->top();
			BinominalyHeap temporary;
			temporary.trees=trees[max_ind]->childs;
			delete trees[max_ind];
			trees[max_ind]=NULL;
			while (trees.size()>0 && max_ind==trees.size()-1) trees.pop_back();
			for (unsigned int i=0;i<temporary.trees.size();i++)
				temporary.trees[i]->parent=NULL;
			merge(temporary);
			return result;
		}
	}

	unsigned int insert(const Type &element)
	{
		BinT* back_pointer=new BinT(element);
		BinominalyHeap<Type,Comparator> fictive_heap;
		fictive_heap.trees.push_back(back_pointer);
		merge(fictive_heap);
		back_pointer->id=adressa.size();
		adressa.push_back(back_pointer);
		return back_pointer->id;
	}

	template <class Type,class Comparator >
	friend void getresult(BinT*& first,BinT*& second,BinT*& carry)
	{
		if (first==NULL && second==NULL) { first=carry;carry=NULL;}
		else if (first==NULL && carry==NULL) { first=second;second=NULL; }
		else if (second==NULL && carry==NULL) return;
		else if (first==NULL) { carry=mergeTrees(second,carry);second=NULL;first=NULL; }
		else if (second==NULL) { carry=mergeTrees(first,carry);second=NULL;first=NULL; }
		else if (carry==NULL) { carry=mergeTrees(first,second);second=NULL;first=NULL; }
		else { 
			BinT* temporary=carry;
			carry=mergeTrees(first,second);
			first=temporary;
			second=NULL;
		}
	}

	void merge(BinominalyHeap<Type,Comparator>& assigned) {
		int q=std::max(trees.size(),assigned.trees.size());
		trees.resize(q);
		assigned.trees.resize(q);
		BinT* carry=NULL;
		for (int i=0;i<q;i++)
			getresult<Type,Comparator>(trees[i],assigned.trees[i],carry);
		if (carry) trees.push_back(carry);
		assigned.trees.resize(0);
	}

	void delete_element(unsigned int pointer_id) 
	{
		BinT* pointer=adressa[pointer_id];
		sift_to_top(pointer,adressa);
		BinominalyHeap temporary;
		temporary.trees=pointer->childs;
		for (unsigned int i=0;i<trees.size();i++)
			if (trees[i])
			if (trees[i]->id==pointer_id) 
			{
				delete trees[i];
				trees[i]=NULL;
			}
		for (unsigned int i=0;i<temporary.trees.size();i++)
			temporary.trees[i]->parent=NULL;
		while (trees.size()>0 && !trees[trees.size()-1]) trees.pop_back();
		merge(temporary);
		adressa[pointer_id]=NULL;
	}

	void change_key(unsigned int& pointer_id,Type newValue)
	{
		Comparator cmp;
		BinT* pointer=adressa[pointer_id];
		if (cmp(newValue,pointer->top()))
		{
			pointer->info=newValue;
			sift_up(pointer,adressa);
		} else {
			delete_element(pointer_id);
			pointer_id=insert(newValue);
		}
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

	Type get_by_id(unsigned int id)
	{
		return adressa[id]->top();
	}

	~BinominalyHeap(){
		for (unsigned int i=0;i<trees.size();i++)
			clear_tree(trees[i]);
		trees.resize(0);
	}
};

template <class Type,class Comparator>
std::vector<BinominalyTree<Type,Comparator>*> BinominalyHeap<Type,Comparator>::adressa;

#endif