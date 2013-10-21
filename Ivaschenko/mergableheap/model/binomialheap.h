#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include "binomialheapnode.h"

template<typename DataType, typename Comparator> class BinomialHeap
{
	typedef BinomialHeapNode<DataType> NodeType;

	public:
		BinomialHeap(): root(nullptr), cmp() {}
		BinomialHeap(Comparator &nCmp): root(nullptr), cmp(nCmp) {}

		~BinomialHeap()
		{
			delete root;
		}

		NodeType* top() const
		{
			if (!root) return nullptr;
			NodeType *answer = root;
			for (NodeType *current = root; current; current = current->getListLink())
				if (!answer || cmp(current->getKey(), answer->getKey()))
					answer = current;
			return answer;
		}

		NodeType* push()
		{
			// TODO: push
		}

		DataType pop()
		{
			// TODO: pop
		}

		void decreaseKey(NodeType *node, DataType newKey)
		{
			// TODO: decrease key
		}

		void erase(NodeType *node)
		{
			// TODO: erase
		}

		void merge(BinomialHeap<DataType> *heap)
		{
			// TODO: merge
		}

	private:
		NodeType *root;
		Comparator cmp;

		void mergeTrees(BinomialHeapNode<DataType> *first, BinomialHeapNode<DataType> *second)
		{
			if (cmp(second->key, first->key)) swap(first, second);
			assert(first->children == second->children);
			assert(first->parent == 0);
			assert(second->parent == 0);
			second->parent = first;
			first->leftChild = second;
			second->listLink
		}
};

#endif // BINOMIALHEAP_H
