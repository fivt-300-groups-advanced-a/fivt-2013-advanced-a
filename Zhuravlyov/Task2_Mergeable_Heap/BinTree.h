#ifndef BIN_TREE
#define BIN_TREE

#include <vector>
#include "unHeap.h"

template < class Type,class Comparator >
class BinominalyTree{
template < class Type,class Comparator > friend class BinominalyHeap;
typedef BinominalyTree<Type,Comparator> BinT;
private:
	std::vector<BinominalyTree<Type,Comparator>*> childs;
	BinominalyTree<Type,Comparator>* parent;
	Type info;
	unsigned int id;
public:
	BinominalyTree(Type element) {
		childs.resize(0);
		parent=NULL;
		info=element;
	}
	  
	friend BinominalyTree* mergeTrees(BinominalyTree<Type,Comparator>* first,BinominalyTree<Type,Comparator>* second)
	{
		Comparator cmp;
		if (!cmp(first->info,second->info)) 
			std::swap(first,second);
		second->parent=first;
		first->childs.push_back(second);
		return first;
	}

	template <class Type,class Comparator >
	friend void sift_up(BinominalyTree<Type,Comparator>*& element,std::vector<BinT*>& adressa) 
	{
		if (element) 
			if (element->parent) {
				Comparator cmp;
				if (cmp(element->info,element->parent->info)) {
					std::swap(element->info,element->parent->info);
					std::swap(adressa[element->id],adressa[element->parent->id]);
					std::swap(element->id,element->parent->id);
					element=element->parent;
					sift_up(element,adressa);
				}
			}
	}

	template <class Type,class Comparator >
	friend void sift_to_top(BinominalyTree<Type,Comparator>*& element,std::vector<BinT*>& adressa) 
	{
		if (element) 
			if (element->parent) {
					std::swap(element->info,element->parent->info);
					std::swap(adressa[element->id],adressa[element->parent->id]);
					std::swap(element->id,element->parent->id);
					element=element->parent;
					sift_to_top(element,adressa);
				}
	}

	template <class Type>
	friend bool check_invariants_for_trees(BinominalyTree<Type,Comparator>* tree)
	{
		if (tree) {
			bool f=1;
			Comparator cmp;
			for (unsigned int i=0;i<tree->childs.size();i++)
				if (!check_invariants_for_trees(tree->childs[i])) f=0;
				else if (!cmp(tree->top(),tree->childs[i]->top())) f=0;
				else if (tree->childs[i]->childs.size()!=i) f=0;
			return f;
		} else return 1;
	}

	template <class Type>
	friend int quantity(BinominalyTree<Type,Comparator>* tree)
	{
		if (!tree) return 0;
		int sum=1;
		for (unsigned int i=0;i<tree->childs.size();i++)
			sum+=quantity(tree->childs[i]);
		return sum;
	}


	template <class Type>
	friend void clear_tree(BinominalyTree<Type,Comparator>* tree)
	{
		if (tree!=NULL) {
			for (unsigned int i=0;i<tree->childs.size();i++)
				clear_tree(tree->childs[i]);
			delete tree;
		}
	}

	Type top()
	{
		return info;
	}
};


#endif