#ifndef STRESS_TEST_H

#define STRESS_TEST_H

#include <gtest/gtest.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <map>

#include "slow_heap.h"
#include "../binomial_heap.h"

/*
 * Stress test of binomial heap with slow version (= vector)
 */

typedef BinomialHeap<int, std::less<int> > Heap;
typedef SlowHeap<int, std::less<int> > Slow;
typedef std::map<int, std::vector<NodeId<int>*> > MapId;

MapId ids;
std::vector<int> values;

inline void check(Heap &heap, Slow &slow_heap){
    ASSERT_EQ(heap.size(), slow_heap.size());
    ASSERT_EQ(heap.empty(), slow_heap.empty());
    if (!heap.empty())
        ASSERT_EQ(heap.getMin(), slow_heap.getMin());
}

void insert(Heap &heap, Slow &slow_heap){
    int cnt = rand() % 1000;
    for (int i = 0; i < cnt; i++){
        int val = rand();
        NodeId<int> *id = heap.insert(val);
        slow_heap.insert(val);
        if (ids.find(val) == ids.end()){
            ids[val] = std::vector<NodeId<int>*>();
        }
        ids[val].push_back(id);
        values.push_back(val);
        check(heap, slow_heap);
    }
}

void merge(Heap &heap, Heap &new_heap, Slow &slow_heap,  Slow &new_slow_heap){
    heap.merge(new_heap);
    slow_heap.merge(new_slow_heap);
    check(heap, slow_heap);
}

void extract(Heap &heap, Slow &slow_heap){
    int cnt = rand();
    for (int i = 0; i < cnt; i++){
        check(heap, slow_heap);
        if (slow_heap.empty())
            break;
        std::pair<int, NodeId<int>*> extracted = heap.extractMin();
        ASSERT_EQ(extracted.first, slow_heap.extractMin());
        for (int j = 0; j < (int)values.size(); j++)
            if (values[j] == extracted.first){
                values.erase(values.begin() + j);
                break;
            }
        bool flag = false;
        MapId::iterator it = ids.find(extracted.first);
        for (int j = 0; j < (int)it->second.size(); j++)
            if (it->second[j] == extracted.second){
                it->second.erase(it->second.begin() + j);
                flag = true;
                break;
            }
        assert(flag);
    }
    check(heap, slow_heap);
}

void create(Heap &heap, Slow &slow_heap){
    Heap new_heap;
    Slow new_slow_heap;
    insert(new_heap, new_slow_heap);
    merge(heap, new_heap, slow_heap, new_slow_heap);
}

void decreaseKey(Heap &heap, Slow &slow_heap){
    int cnt = rand() % 10000;
    if (heap.empty())
        return;
    for (int i = 0; i < cnt; i++){
        int pos = rand() % values.size();
        int old_val = values[pos];
        int delta = rand() % 10000;
        int new_val = old_val - delta;
        values[pos] = new_val;
        slow_heap.decreaseKey(old_val, new_val);
        NodeId<int> *id = *ids[old_val].begin();
        ids[old_val].erase(ids[old_val].begin());
        ids[new_val].push_back(id);
        heap.decreaseKey(id, new_val);
        check(heap, slow_heap);
    }
}

TEST(StressTest, TestWithVector){
    Heap heap;
    Slow slow_heap;

    //int k = time(0);
    int k = 1385158347;
    srand(k);
    std::cerr << k << std::endl;

    //insert some elements
    insert(heap, slow_heap);

    // create another heap, insert elements there and merge with previous
    create(heap, slow_heap);

    // extract some elements
    extract(heap, slow_heap);

    // insert some elements
    insert(heap, slow_heap);

    // decrease some keys
    decreaseKey(heap, slow_heap);

    // insert some elements
    insert(heap, slow_heap);

    // decrease again
    decreaseKey(heap, slow_heap);

    // create another heap, insert elements there and merge with previous
    create(heap, slow_heap);

    for (int i = 0; i < 20; i++){
        insert(heap, slow_heap);
        decreaseKey(heap, slow_heap);
        insert(heap, slow_heap);
        extract(heap, slow_heap);
        decreaseKey(heap, slow_heap);
        decreaseKey(heap, slow_heap);
        insert(heap, slow_heap);
        create(heap, slow_heap);
        create(heap, slow_heap);
        decreaseKey(heap, slow_heap);
        extract(heap, slow_heap);
    }


}


#endif
