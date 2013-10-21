#ifndef HEAPNODE_H
#define HEAPNODE_H

#include <utility>

template<typename DataType> class BinomialHeapNode
{
	friend class BinomialHeap;

	public:
		explicit BinomialHeapNode(const DataType &nKey): key(nKey), parent(0), listNext(0), leftChild(0), children(0)
		{
		}

		~BinomialHeapNode()
		{
			if (leftChild) delete leftChild;
			if (listNext) delete listNext;
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

		/**
		 * We cannot split key and pointer to the node
		 * So let us swap all other parameters
		 */
		void swap(BinomialHeapNode &node)
		{
			swap(parent, node.parent);
			swap(listLink, node.listLink);
			swap(leftChild, node.leftChild);
			swap(children, node.children);
		}

	private:
		DataType key;
		BinomialHeapNode *parent, *listLink, *leftChild;
		std::size_t children;
};

#endif // HEAPNODE_H
