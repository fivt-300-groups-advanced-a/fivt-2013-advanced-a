#ifndef INVARIANT_TEST_H

#define INVARIANT_TEST_H

#include <gtest/gtest.h>
#include <map>

#include "test_heap.h"
#include "../binomial_heap.h"
#include "testing_utilities.h"

namespace InvariantTesting {

    typedef TestHeap<int, std::less<int> > MyTestHeap;
    typedef NodeId<int>* NodeIdPtr;

    std::map<int, std::vector<NodeIdPtr> > ids;
    std::vector<int> values;

    void insert(MyTestHeap &heap){
        int cnt = rand() % 1000;
        for (int i = 0; i < cnt; i++){
            int val = rand();
            ids[val].push_back(heap.insert(val));
            values.push_back(val);
            ASSERT_TRUE(heap.checkInvariant());
        }
    }

    void extractMin(MyTestHeap &heap){
        int cnt = rand() % 1000;
        for (int i = 0; i < cnt; i++){
            if (heap.empty())
                break;
            std::pair<int, NodeIdPtr> mn = heap.extractMin();
            int val = mn.first;
            NodeIdPtr id = mn.second;
            eraseFromVector<int, std::less<int> >(values, val);
            eraseFromVector<NodeIdPtr, std::less<NodeIdPtr> >(ids[val], id); 
            ASSERT_TRUE(heap.checkInvariant());
        }
    }

    void merge(MyTestHeap &heap){
        MyTestHeap new_heap;
        insert(new_heap);
        ASSERT_TRUE(new_heap.checkInvariant());
        heap.merge(new_heap);
        ASSERT_TRUE(heap.checkInvariant());
    }

    void decreaseKey(MyTestHeap &heap){
        int cnt = rand() % 1000;
        for (int i = 0; i < cnt; i++){
            if (heap.empty())
                return;
            int pos = rand() % values.size();
            int val = values[pos];
            NodeIdPtr id = *ids[val].begin();
            ids[val].erase(ids[val].begin());
            int delta = rand() % 10000;
            int new_val = val - delta;
            values[pos] = new_val;
            ids[new_val].push_back(id);
            heap.decreaseKey(id, new_val);
            ASSERT_TRUE(heap.checkInvariant());
        }
    }

    void erase(MyTestHeap &heap){
        int cnt = rand() % 1000;
        for (int i = 0; i < cnt; i++){
            if (heap.empty())
                return;
            int pos = rand() % values.size();
            int val = values[pos];
            eraseFromVector<int, std::less<int> >(values, val);
            heap.erase(*ids[val].begin());
            ids[val].erase(ids[val].begin());
            ASSERT_TRUE(heap.checkInvariant());
        }
    }

    /*
     * Check if BinomialHeap is really binomial heap
     */
    TEST(InvariantTest, TestHeap){
        srand(time(0));
        MyTestHeap heap;

        insert(heap);
        extractMin(heap);
        insert(heap);
        insert(heap);
        extractMin(heap);
        merge(heap);
        merge(heap);
        extractMin(heap);
        merge(heap);
        decreaseKey(heap);
        insert(heap);
        insert(heap);
        decreaseKey(heap);
        decreaseKey(heap);
        erase(heap);
        erase(heap);
        insert(heap);
        merge(heap);
        merge(heap);
        insert(heap);
        erase(heap);
        for (int i = 0; i < 10; i++){
            erase(heap);
            merge(heap);
            insert(heap);
            decreaseKey(heap);
            extractMin(heap);
        }
    }

}
#endif
