#ifndef HEAPNODE_H
#define HEAPNODE_H

#include <utility>

template<typename DataType, typename Comparator> class BinomialHeap;

template<typename DataType, typename Comparator> class BinomialHeapNode
{
	friend class BinomialHeap<DataType, Comparator>;

	public:
		explicit BinomialHeapNode(const DataType &nKey): key(nKey), parent(0),
														 listLink(0), leftChild(0), children(0) {}

		~BinomialHeapNode()
		{
			if (leftChild) delete leftChild;
			if (listLink) delete listLink;
		}

		const BinomialHeapNode* getListLink() const
		{
			return listLink;
		}

		const BinomialHeapNode* getParent() const
		{
			return parent;
		}

		const BinomialHeapNode* getLeftChild() const
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
		std::size_t children;

		/**
		 * @brief swap We cannot split key and pointer to the node so let us swap all other parameters
		 * @param node node to swap with
		 */
		void swap(BinomialHeapNode<DataType, Comparator> &node)
		{
			swap(parent, node.parent);
			swap(listLink, node.listLink);
			swap(leftChild, node.leftChild);
			swap(children, node.children);
		}

		void merge(BinomialHeapNode<DataType, Comparator> *node)
		{
			assert(children == node->children);
			assert(parent == 0);
			assert(node->parent == 0);

			node->parent = this;
			node->listLink = leftChild;
			leftChild = node;
			++children;
		}
};

#endif // HEAPNODE_H
