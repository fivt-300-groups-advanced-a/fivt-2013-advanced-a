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

		bool empty() const
		{
			return !root;
		}

		void clear()
		{
			delete root;
			root = 0;
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

		NodeType* push(const DataType &value)
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

		/**
		 * @brief merge merges one binomial heap to another in O(log n + log m) time.
		 * Pointers of the second heap are still valid in another, but the second heap becomes empty
		 * @param heap a heap to merge with
		 */
		void merge(BinomialHeap<DataType, Comparator> &heap)
		{
			if (!heap.root) return;
			if (!root)
			{
				root = heap.root;
				return;
			}
			NodeType *cur = 0, *start = 0;
			for (NodeType *first = root, *second = heap.root; first && second;)
				if (first->getRank() < second->getRank())
				{
					if (cur) cur->listLink = first;
					else start = first;
					cur = first;
					first = first->listLink;
				}
				else
				{
					if (cur) cur->listLink = second;
					else start = second;
					cur = second;
					second = second->listLink;
				}

			for (cur = start; cur->listLink;)
				if (cur->getRank() == cur->listLink->getRank())
				{
					NodeType *nextNode = cur->listLink;
					mergeTrees(cur->listLink, cur);
					cur = nextNode;
				}
				else cur = cur->listLink;
			heap.root = 0;
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
			second->listLink = first->leftChild;
			first->leftChild = second;
			++first->children;
		}
};

#endif // BINOMIALHEAP_H
