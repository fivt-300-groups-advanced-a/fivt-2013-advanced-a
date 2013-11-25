#ifndef TEST_WITH_POINT_CLASS_H

#define TEST_WITH_POINT_CLASS_H

#include <gtest/gtest.h>
#include <cstdio>
#include <ctime>

#include "testing_utilities.h"
#include "slow_heap.h"
#include "../binomial_heap.h"
#include "point.h"

/* 
 * Testing BinomialHeap of Point
 * Comparator - by distancce from a given point
 */
TEST(PointsTest, StressWithSlowHeap){
    PointComparator cmp(9, 92);
    RandomPointGenerator gen(cmp);
    BinomialHeap<Point, PointComparator> heap(cmp);
    SlowHeap<Point, PointComparator> slow_heap(cmp);
    Tester<Point, PointComparator, RandomPointGenerator> 
        tester(cmp, gen);

    tester.insert(heap, slow_heap);
    tester.insert(heap, slow_heap);
    tester.merge(heap, slow_heap);
    tester.merge(heap, slow_heap);
    tester.extractMin(heap, slow_heap);
    tester.insert(heap, slow_heap);
    tester.decreaseKey(heap, slow_heap);
    tester.erase(heap, slow_heap);

    for (int i = 0; i < 20; i++){
        tester.insert(heap, slow_heap);
        tester.erase(heap, slow_heap);
        tester.decreaseKey(heap, slow_heap);
        tester.merge(heap, slow_heap);
        tester.decreaseKey(heap, slow_heap);
    }

}
#endif
