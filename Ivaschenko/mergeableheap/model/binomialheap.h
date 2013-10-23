#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include "binomialheapnode.h"

/**
 *
 */
template<typename DataType> class BinomialHeapNodeIdentifier
{
	friend class BinomialHeap;

	public:
		/**
		 * @brief BinomialHeapNodeIdentifier
		 * @param nNode
		 */
		explicit BinomialHeapNodeIdentifier(NodeType *nNode): node(nNode) {}

		/**
		 * @brief exist
		 * @return
		 */
		bool exist() const
		{
			return node != nullptr;
		}

		/**
		 * @brief operator *
		 * @return
		 */
		const DataType& operator * () const
		{
			return *node;
		}

	private:
		typedef BinomialHeapNode<DataType> NodeType;

		NodeType *node;
};

/**
 * Class representing binomial heap - a mergeable data structure that can do following operations in O(log n) time
 *	 - add element
 *   - find minimum
 *	 - extract minimum
 *	 - merge two structures in O(log n + log m) time
 *	 - erase element
 *	 - decrease value of element
 * Uses user-defined comparator
 */
template<typename DataType, typename Comparator> class BinomialHeap
{
	public:
		typedef BinomialHeapNodeIdentifier<DataType> NodeIdType;

		/**
		 * @brief BinomialHeap creates new empty heap with default Comparator
		 */
		BinomialHeap(): root(nullptr), cmp() {}

		/**
		 * @brief BinomialHeap creates new empty heap with specified Comparator
		 * @param nCmp comparator
		 */
		explicit BinomialHeap(Comparator &nCmp): root(nullptr), cmp(nCmp) {}

		~BinomialHeap()
		{
			clear();
		}

		/**
		 * @brief empty checks whether heap is empty now
		 * @return true if heap contains no element
		 */
		bool empty() const
		{
			return !root;
		}

		/**
		 * @brief clear erases all elements from heap
		 * Complexity: O(n)
		 */
		void clear()
		{
			delete root;
			root = 0;
		}

		/**
		 * @brief top finds the smallest element in heap
		 * Complexity O(log n)
		 * @return identifier of the smallest element
		 */
		NodeIdType top() const
		{
			if (!root) return nullptr;
			NodeType *answer = root;
			for (NodeType *current = root; current; current = current->listLink)
				if (!answer || cmp(current->key, answer->key))
					answer = current;
			return NodeIdType(answer);
		}

		/**
		 * @brief push adds one element to the heap
		 * Complexity: O(log n)
		 * @param value element itself
		 * @return identifier of heap node corresponding to added value
		 */
		NodeIdType push(const DataType &value)
		{
			NodeType *newNode = new NodeType(value), reference = newNode;
			mergeHeaps(newNode);
			return NodeIdType(reference);
		}

		/**
		 * @brief pop finds the smallest element in heap and erases it
		 * Complexity: O(log n)
		 * @return value of the smallest element
		 */
		DataType pop()
		{
			assert(!empty());
			NodeType *answer = root, *prev = 0;
			for (NodeType *current = root; current->listLink; current = current->listLink)
				if (!answer || cmp(current->listLink->key, answer->key))
					answer = current->listLink, prev = current; // find minimum
			DataType minValue(answer->key);
			eraseNode(prev, answer);
			return minValue; // return value
		}

		/**
		 * @brief decreaseKey decreases value of specified element
		 * Complexity: O(log n)
		 * @param nodeId identifier of element
		 * @param newKey it's new value
		 */
		void decreaseKey(const NodeIdType &nodeId, const DataType &newKey)
		{
			assert(node.exist());
			NodeType *node = nodeId.node;
			node->key = newKey;
			while (node->parent)
			{
				if (!cmp(node->key, node->parent->key)) break;
				node->swap(parent);
			}
		}

		/**
		 * @brief erase erases specified element from heap
		 * Complexity: O(log n)
		 * @param nodeId identifier of element
		 */
		void erase(const NodeIdType &nodeId)
		{
			assert(nodeId.exist());
			NodeType *node = nodeId.node, prev = 0;
			while (node->parent)
				node->swap(parent);
			for (NodeType *current = root; current->listLink; current = current->listLink)
				if (current->listLink == node)
				{
					prev = current;
					break;
				}
			eraseNode(prev, node);
		}

		/**
		 * @brief merge merges one binomial heap to another.
		 * Pointers of the second heap are still valid in another, but the second heap becomes empty
		 * Complexity: O(log n + log m)
		 * @param heap a heap to merge with
		 */
		void merge(BinomialHeap<DataType, Comparator> &heap)
		{
			mergeHeaps(heap.root);
		}

	private:
		typedef BinomialHeapNode<DataType> NodeType;

		NodeType *root;
		Comparator cmp;

		void mergeHeaps(NodeType* &node)
		{
			if (!node) return;
			if (!root)
			{
				root = node;
				return;
			}
			for (NodeType *cur = 0, *first = root, *second = node; first && second;) // merge two lists
				if (first->getRank() > second->getRank()) // from high rank to low
				{
					if (cur) cur->listLink = first;
					else root = first;
					cur = first;
					first = first->listLink;
				}
				else
				{
					if (cur) cur->listLink = second;
					else root = second;
					cur = second;
					second = second->listLink;
				}

			for (NodeType *prev = 0, *cur = root; cur->listLink;)
				if (cur->getRank() == cur->listLink->getRank()) // merge trees with equal rank
				{
					NodeType *nextNode;
					if (cmp(cur->listLink->key, cur->key))
					{
						if (prev) prev->listLink = cur->listLink;
						nextNode = cur->listLink;
						cur->listLink->merge(cur);
					}
					else
					{
						cur->listLink = cur->listLink->listLink;
						nextNode = cur;
						cur->merge(cur->listLink);
					}
					cur = nextNode;
				}
				else cur = cur->listLink; // proceed
			node = 0; // second heap is now empty
		}

		void eraseNode(NodeType *prev, NodeType *node)
		{
			for (NodeType *child = node->leftChild; child; child = child->listLink)
				child->parent = 0; // children now have no parent

			if (prev) prev->listLink = node->listLink; // cut off from list
			else root = node->listLink;

			mergeHeaps(node->leftChild); // merge children to root list
			node->leftChild = 0; // has no children more
			delete node; // free memory
		}
};

#endif // BINOMIALHEAP_H
