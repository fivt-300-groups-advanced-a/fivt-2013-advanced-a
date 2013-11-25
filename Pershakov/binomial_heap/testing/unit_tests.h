#ifndef UNIT_TEST_H

#define UNIT_TEST_H

#include <vector>
#include <gtest/gtest.h>

#include "../binomial_heap.h"

std::vector<NodeId<int>*> 
addInterval(BinomialHeap<int, std::less<int> > &heap, int l, int r){
    std::vector<NodeId<int>*> ans;
    for (int i = l; i <= r; i++){
        ans.push_back(heap.insert(i));
    }
    return ans;
}

TEST(UnitTest, InsertExtractEmptySize){
    BinomialHeap<int, std::less<int> > heap;
    int n = 100000;
    for (int i = 0; i < n; i++){
        heap.insert(i);
        ASSERT_EQ(heap.size(), i + 1);
    }
    for (int i = 0; i < n; i++){
        int mn = heap.getMin();
        heap.extractMin();
        ASSERT_EQ(mn, i);
        ASSERT_EQ(heap.size(), n - i - 1);
    }
    ASSERT_TRUE(heap.empty());
}

TEST(UnitTest, MergeEmptySize){
    BinomialHeap<int, std::less<int> > heap1, heap2;
    int n = 100000;
    addInterval(heap1, 0, n - 1);
    addInterval(heap2, n, 2 * n - 1);
    heap1.merge(heap2);
    ASSERT_EQ(heap1.size(), 2 * n);
    for (int i = 0; i < 2 * n; i++){
        int mn = heap1.getMin();
        heap1.extractMin();
        ASSERT_EQ(mn, i);
    }
    ASSERT_TRUE(heap1.empty());
}

TEST(UnitTest, DecreaseMergeEmptySize){
    BinomialHeap<int, std::less<int> > heap1, heap2;
    int n = 100000;
    addInterval(heap1, 0, n - 1);
    std::vector<NodeId<int>*> ids = addInterval(heap2, n, 2 * n - 1);
    heap1.merge(heap2);
    for (int i = 0; i < (int)ids.size(); i++)
        heap1.decreaseKey(ids[i], i);
    for (int i = 0; i < 2 * n; i++){
        ASSERT_EQ(heap1.getMin(), i / 2);
        heap1.extractMin();
    }
    ASSERT_TRUE(heap1.empty());
}

TEST(UnitTest, EraseEmptySize){
    BinomialHeap<int, std::less<int> > heap;
    std::vector<NodeId<int>*> ids;
    int n = 100000;
    ids = addInterval(heap, 0, n - 1);
    for (int i = 0; i < n; i++)
        ids.push_back(heap.insert(i));
    for (int i = 0; i < n; i++)
        heap.erase(ids[i]);
    ASSERT_EQ(heap.size(), n);
    for (int i = 0; i < n; i++){
        ASSERT_EQ(heap.getMin(), i);
        heap.extractMin();
    }
    ASSERT_TRUE(heap.empty());
}

#endif
