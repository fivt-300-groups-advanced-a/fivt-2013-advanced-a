#ifndef TESTACCESS_H
#define TESTACCESS_H

#include <set>

#include <gtest/gtest.h>

#include "binomialheap.h"
#include "binomialheapnode.h"

template<typename Class> class TestAccess {};

template<typename DataType, typename Comparator>
class TestAccess< BinomialHeap<DataType, Comparator> >
{
	public:
		typedef BinomialHeap<DataType, Comparator> HeapType;
		typedef typename HeapType::NodeType NodeType;

		void checkInvariants(const HeapType &heap)
		{
			if (heap.empty())
			{
				ASSERT_TRUE(heap.root == nullptr);
				ASSERT_TRUE(heap.size() == 0);
				return;
			}
			visited.clear();
			std::size_t size = 0;
			for (NodeType *current = heap.root; current; current = current->getListLink())
			{
				std::size_t add = traverseHeap(current, nullptr, heap);
				ASSERT_EQ(add & (add - 1), 0);
				ASSERT_EQ(size & add, 0);
				size += add;
			}
			ASSERT_TRUE(size == heap.size());
		}

	private:
		std::set<long long> visited;

		inline long long getNodeId(NodeType *node)
		{
			return (long long) node;
		}

		std::size_t traverseHeap(NodeType *node, NodeType *parent, const HeapType &heap)
		{
			if (parent) EXPECT_FALSE(heap.cmp(node->getKey(), parent->getKey()));
			EXPECT_EQ(node->getParent(), parent);
			EXPECT_EQ(visited.count(getNodeId(node)), 0);
			visited.insert(getNodeId(node));
			EXPECT_EQ(visited.count(getNodeId(node->getListLink())), 0);
			std::size_t size = 1, sizeCount = node->getRank(), childCount = 0;
			for (NodeType *child = node->getLeftChild(); child; child = child->getListLink())
			{
				--sizeCount;
				++childCount;
				EXPECT_EQ(child->getRank(), sizeCount);
				size += traverseHeap(child, node, heap);
			}
			EXPECT_EQ(childCount, node->getRank());
			EXPECT_EQ(1 << node->getRank(), size);
			return size;
		}
};

#endif // TESTACCESS_H
