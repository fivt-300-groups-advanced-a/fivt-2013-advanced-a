#ifndef FRIENDS
#define FRIENDS

#include "BinTree.h"


	template <class Type,class Comparator>
	BinomialTree<Type,Comparator>* mergeTrees(BinomialTree<Type,Comparator>*& first,
											  BinomialTree<Type,Comparator>*& second)
	{
		Comparator cmp;
		if (!cmp(first->info,second->info)) 
			std::swap(first,second);
		second->parent=first;
		first->childs.push_back(second);
		return first;
	}

	template <class Type,class Comparator >
	void replace(BinomialTree<Type,Comparator>*& low,
				 BinomialTree<Type,Comparator>*& top)
	{
		std::swap(low->info,top->info);
		std::swap(low->backward_pointer->node,top->backward_pointer->node);
		std::swap(low->backward_pointer,top->backward_pointer);
		low=top;
	}

	template <class Type,class Comparator >
	void sift_up(BinomialTree<Type,Comparator>* element) 
	{
		if (element) { 
			Comparator cmp;
			while (element->parent && cmp(element->info,element->parent->info)) 
				replace(element,element->parent);
		}
	}

	template <class Type,class Comparator>
	void sift_to_top(BinomialTree<Type,Comparator>* element) 
	{
		if (element) 
			while (element->parent) 
				replace(element,element->parent);
	}

	template <class Type,class Comparator>
	bool check_invariants_for_trees(BinomialTree<Type,Comparator>* tree)
	{
		if (tree) {
			bool f=1;
			Comparator cmp;
			for (unsigned int i=0;i<tree->childs.size();i++)
				if (!check_invariants_for_trees(tree->childs[i])) f=0;
				else if (cmp(tree->childs[i]->top(),tree->top())) f=0;
				else if (tree->childs[i]->childs.size()!=i) f=0;
				else if (tree->childs[i]->parent!=tree) f=0;
			return f;
		} else return 1;
	}

	template <class Type,class Comparator>
	int quantity(BinomialTree<Type,Comparator>* tree)
	{
		if (!tree) return 0;
		int sum=1;
		for (unsigned int i=0;i<tree->childs.size();i++)
			sum+=quantity(tree->childs[i]);
		return sum;
	}

	template <class Type,class Comparator>
	void clear_tree(BinomialTree<Type,Comparator>* tree)
	{
		if (tree!=NULL) {
			for (unsigned int i=0;i<tree->childs.size();i++)
				clear_tree(tree->childs[i]);
			delete tree;
		}
	}

	template <class Type,class Comparator >
	void getresult(BinomialTree<Type,Comparator>*& first,
				   BinomialTree<Type,Comparator>*& second,
				   BinomialTree<Type,Comparator>*& carry)
	{
		if (second==NULL && carry==NULL) return;
		if (first==NULL && second==NULL) { first=carry;carry=NULL;}
		else if (first==NULL && carry==NULL) { first=second;second=NULL; }
		else if (first==NULL) { carry=mergeTrees(second,carry);second=NULL;first=NULL; }
		else if (second==NULL) { carry=mergeTrees(first,carry);second=NULL;first=NULL; }
		else if (carry==NULL) { carry=mergeTrees(first,second);second=NULL;first=NULL; }
		else { 
			BinomialTree<Type,Comparator>* temporary=carry;
			carry=mergeTrees(first,second);
			first=temporary;
			second=NULL;
		}
	}
	
#endif