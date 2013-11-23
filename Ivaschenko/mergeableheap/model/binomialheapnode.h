#ifndef HEAPNODE_H
#define HEAPNODE_H

#include <utility>
#include <list>

template<typename DataType, typename Comparator> class BinomialHeap;
template<typename DataType, typename Comparator> class BinomialHeapIndex;

template<typename DataType, typename Comparator> class BinomialHeapNode
{
	friend class BinomialHeap<DataType, Comparator>;

	private:
		typedef typename std::list<BinomialHeapNode*>::iterator IndexType;

	public:
		explicit BinomialHeapNode(const DataType &nKey, IndexType nIndex): key(nKey), parent(0),
																			listLink(0), leftChild(0), index(nIndex),
																			children(0) {}

		~BinomialHeapNode()
		{
			if (leftChild) delete leftChild;
			if (listLink) delete listLink;
		}

		BinomialHeapNode* getListLink() const
		{
			return listLink;
		}

		BinomialHeapNode* getParent() const
		{
			return parent;
		}

		BinomialHeapNode* getLeftChild() const
		{
			return leftChild;
		}

		const DataType& getKey() const
		{
			return key;
		}

		std::size_t getRank() const
		{
			return children;
		}

	private:
		DataType key;
		BinomialHeapNode *parent, *listLink, *leftChild;
		IndexType index;
		std::size_t children;

		/**
		 * @brief swap We cannot split key and pointer to the node so let us swap all other parameters
		 * @param node node to swap with
		 */
		void swap(BinomialHeapNode<DataType, Comparator> &node)
		{
			std::swap(key, node.key);
			std::swap(index, node.index);
		}

		void merge(BinomialHeapNode<DataType, Comparator> *node)
		{
			node->parent = this;
			node->listLink = leftChild;
			leftChild = node;
			++children;
		}
};

#endif // HEAPNODE_H
