#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include "binomialheapnode.h"

#include <list>

template <typename DataType, typename Comparator> class BinomialHeap;
template <typename DataType, typename Comparator> class BinomialHeapNodeIdentifier;
template <typename DataType, typename Comparator> class BinomialHeapIndex;
template <typename Class> class TestAccess;

/**
 * Class representing const reference to a heap node
 * Identifier is always associated with data pushed into heap from creation to deletion
 */
template<typename DataType, typename Comparator> class BinomialHeapNodeIdentifier
{
	friend class BinomialHeap<DataType, Comparator>;

	private:
		typedef BinomialHeapNode<DataType, Comparator> NodeType;
		typedef typename std::list<NodeType*>::iterator IndexType;

	public:
		/**
		 * @brief BinomialHeapNodeIdentifier creates new identifier from pointer to node
		 * @param nNode pointer to node
		 */
		explicit BinomialHeapNodeIdentifier(IndexType nIndex): index(nIndex) {}

		/**
		 * @brief operator * provides access to data in heap
		 * @return const reference to stored data
		 */
		const DataType& operator * () const
		{
			return (*index)->getKey();
		}

	private:
		IndexType index;
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
template<typename DataType, typename Comparator = std::less<DataType> > class BinomialHeap
{
	friend class TestAccess< BinomialHeap<DataType, Comparator> >;

	private:
		typedef BinomialHeapNode<DataType, Comparator> NodeType;
		typedef typename std::list<NodeType*>::iterator IndexType;

	public:
		typedef BinomialHeapNodeIdentifier<DataType, Comparator> NodeIdType;

		/**
		 * @brief BinomialHeap creates new empty heap with default Comparator
		 */
		BinomialHeap(): root(nullptr), indexList(), cmp() {}

		/**
		 * @brief BinomialHeap creates new empty heap with specified Comparator
		 * @param nCmp comparator
		 */
		explicit BinomialHeap(Comparator nCmp): root(nullptr), indexList(), cmp(nCmp) {}

		~BinomialHeap()
		{
			clear();
		}

		std::size_t size() const
		{
			return indexList.size();
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
			if (root) delete root;
			root = 0;

			indexList.clear();
		}

		/**
		 * @brief top finds the smallest element in heap
		 * Complexity O(log n)
		 * @return identifier of the smallest element
		 */
		DataType top() const
		{
			assert(!empty());
			NodeType *answer = root;
			for (NodeType *current = root; current; current = current->listLink)
				if (!answer || cmp(current->key, answer->key))
					answer = current;
			return answer->getKey();
		}

		/**
		 * @brief push adds one element to the heap
		 * Complexity: O(log n)
		 * @param value element itself
		 * @return identifier of heap node corresponding to added value
		 */
		NodeIdType push(const DataType &value)
		{
			indexList.push_front(nullptr);
			NodeType *newNode = new NodeType(value, indexList.begin());
			indexList.front() = newNode;
			mergeHeaps(newNode);
			return NodeIdType(indexList.begin());
		}

		/**
		 * @brief pop finds the smallest element in heap and erases it. Identifiers are not affected
		 * except one for deleted element
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
		 * @brief decreaseKey decreases value of specified element. Identifiers are not invalidated
		 * Complexity: O(log n)
		 * @param id identifier of element
		 * @param newKey it's new value
		 */
		void decreaseKey(const NodeIdType &id, const DataType &newKey)
		{
			NodeType *node = *id.index;
			node->key = newKey;
			while (node->parent)
			{
				if (!cmp(node->key, node->parent->key)) break;
				swapElementsData(node, node->parent);
				node = node->parent;
			}
		}

		/**
		 * @brief erase erases specified element from heap. All indentifiers (except deleted) are not affected
		 * Complexity: O(log n)
		 * @param id identifier of element
		 */
		void erase(const NodeIdType &id)
		{
			NodeType *node = *id.index, *prev = 0;
			while (node->parent)
			{
				swapElementsData(node, node->parent);
				node = node->parent;
			}

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
		 * Identifiers of the second heap are still valid in another
		 * Important: second heap becomes empty
		 * Complexity: O(log n + log m)
		 * @param heap a heap to merge with
		 */
		void merge(BinomialHeap<DataType, Comparator> &heap)
		{
			indexList.splice(indexList.begin(), heap.indexList); // Constant time merge lists
			mergeHeaps(heap.root);
		}

	private:
		NodeType *root;
		std::list<NodeType*> indexList;
		Comparator cmp;

		/**
		 * @brief swapElementsData performs swap of two nodes without splitting a key and pointer to node
		 * Complexity: O(1)
		 * @param first first node
		 * @param second second node
		 */
		void swapElementsData(NodeType* first, NodeType* second)
		{
			swap(*first->index, *second->index);
			first->swap(*second);
		}

		/**
		 * @brief reverseList reverses list of siblings of node (root list is stored in increasing oreder
		 * while children list of each vertex is stored in decreasing order)
		 * Complexity: O(log n)
		 * @param node head of list of siblings
		 * @return head of reversed list
		 */
		NodeType* reverseList(NodeType *node)
		{
			if (!node) return nullptr;
			NodeType *prev = 0;
			for (NodeType *current = node;;)
			{
				NodeType *nextNode = current->listLink;
				current->listLink = prev;
				prev = current;
				if (!nextNode) return current;
				current = nextNode;
			}
		}

		/**
		 * @brief append appends one list to another
		 * @param node
		 * @param appendix
		 */
		void append(NodeType* &node, NodeType *appendix)
		{
			if (node) node->listLink = appendix;
			else root = node = appendix;
		}

		/**
		 * @brief mergeHeaps merges given heap to own
		 * Complexity: O(log n)
		 * @param node pointer to heap.root of second heap
		 */
		void mergeHeaps(NodeType* &node)
		{
			if (!node) return;
			if (!root)
			{
				root = node;
				return;
			}
			NodeType *first = root, *second = node, *temp = 0, *cur = 0;
			for (std::size_t rnk = 0; first || second; ++rnk)
			{
				NodeType *copyFirst = 0, *copySecond = 0;
				if (first && first->getRank() == rnk)
				{
					copyFirst = first;
					first = first->listLink;
					copyFirst->listLink = 0;
				}
				if (second && second->getRank() == rnk)
				{
					copySecond = second;
					second = second->listLink;
					copySecond->listLink = 0;
				}

				if (copyFirst && copySecond)
				{
					append(cur, temp);
					if (!cmp(copyFirst->key, copySecond->key))
						swap(copyFirst, copySecond);
					copyFirst->merge(copySecond);
					temp = copyFirst;
				}
				else if (!copyFirst && !copySecond)
				{
					append(cur, temp);
					temp = 0;
				}
				else
				{
					if (!copyFirst) swap(copyFirst, copySecond);
					if (temp)
					{
						if (!cmp(copyFirst->key, temp->key)) swap(copyFirst, temp);
						copyFirst->merge(temp);
						temp = copyFirst;
					}
					else append(cur, copyFirst);
				}
				if (cur && cur->listLink) cur = cur->listLink;
			}
			append(cur, temp);
			node = 0; // second heap is now empty
		}

		/**
		 * @brief eraseNode erases node from heap and merges it's children to entire heap
		 * Complexity: O(log n)
		 * @param prev previous element
		 * @param node element to delete
		 */
		void eraseNode(NodeType *prev, NodeType *node)
		{
			for (NodeType *child = node->leftChild; child; child = child->listLink)
				child->parent = 0; // children now have no parent

			if (prev) prev->listLink = node->listLink; // cut off from list
			else root = node->listLink;

			node->leftChild = reverseList(node->leftChild);
			mergeHeaps(node->leftChild); // merge children to root list
			node->listLink = 0;  // has no next element more
			node->leftChild = 0; // has no children more

			indexList.erase(node->index); // delete reference to node from index
			delete node; // kill him! free memory!
		}
};

#endif // BINOMIALHEAP_H
