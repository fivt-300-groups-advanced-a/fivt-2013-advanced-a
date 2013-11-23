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
#include "testing_utilities.h"

class RandomIntGenerator {
    public:
        RandomIntGenerator(){
            srand(time(0));
        }

        int operator() (){
            return rand();
        }

        int decrease(int key){
            int delta = rand() % 10000;
            return key - delta;
        }
};

/*
 * Stress test of binomial heap with slow version (= vector)
 */
TEST(StressTest, TestWithVector){
    typedef BinomialHeap<int, std::less<int> > Heap;
    typedef SlowHeap<int, std::less<int> > Slow;
    Tester<int, std::less<int>, RandomIntGenerator, 
        std::hash<int>, std::equal_to<int> > tester;
    Heap heap;
    Slow slow_heap;

    tester.insert(heap, slow_heap);
    tester.merge(heap, slow_heap);
    tester.extractMin(heap, slow_heap);
    tester.insert(heap, slow_heap);
    tester.erase(heap, slow_heap);
    tester.decreaseKey(heap, slow_heap);
    tester.insert(heap, slow_heap);
    tester.decreaseKey(heap, slow_heap);
    tester.erase(heap, slow_heap);
    tester.merge(heap, slow_heap);

    for (int i = 0; i < 20; i++){
        tester.insert(heap, slow_heap);
        tester.decreaseKey(heap, slow_heap);
        tester.insert(heap, slow_heap);
        tester.extractMin(heap, slow_heap);
        tester.decreaseKey(heap, slow_heap);
        tester.decreaseKey(heap, slow_heap);
        tester.insert(heap, slow_heap);
        tester.merge(heap, slow_heap);
        tester.merge(heap, slow_heap);
        tester.erase(heap, slow_heap);
        tester.decreaseKey(heap, slow_heap);
        tester.extractMin(heap, slow_heap);
    }
    
    while (!heap.empty()){
        tester.check(heap, slow_heap);
        heap.extractMin();
        slow_heap.extractMin();
    }
}

#endif
