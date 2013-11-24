#ifndef BIN_TREE
#define BIN_TREE

#include <vector>
#include "BinHeap.h"

template <class Structure >
struct ValueReference
{
	Structure* node;
};

template <class Type,class Comparator>
class BinomialTree{

private:
	typedef BinomialTree<Type,Comparator> BinT;
	template <class Type,class Comparator> friend class BinomialHeap;
	std::vector<BinomialTree<Type,Comparator>*> childs;
	BinomialTree<Type,Comparator>* parent;
	Type info;
	ValueReference<BinT>* backward_pointer;

public:
	BinomialTree(Type element) {
		childs.resize(0);
		parent=NULL;
		info=element;
	}

	template <class Type,class Comparator> friend BinT* mergeTrees(BinT*& first,BinT*& second);
	template <class Type,class Comparator> friend void sift_up(BinT* element);
	template <class Type,class Comparator> friend void sift_to_top(BinT* element) ;
	template <class Type,class Comparator> friend bool check_invariants_for_trees(BinT* tree);
	template <class Type,class Comparator> friend int quantity(BinT* tree);
	template <class Type,class Comparator> friend void clear_tree(BinT* tree);
	template <class Type,class Comparator> friend void replace(BinT*& low,BinT*& top);
	template <class Type,class Comparator> friend void refresh_parents(BinT*& pointer);
	template <class Type,class Comparator> friend void getresult(BinT*& first,BinT*& second,BinT*& carry);

	Type top()
	{
		return info;
	}

	~BinomialTree()
	{
		delete backward_pointer;
	}
};

#endif