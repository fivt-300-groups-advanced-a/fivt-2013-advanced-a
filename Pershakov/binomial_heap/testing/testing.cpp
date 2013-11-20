#include <vector>
#include <algorithm>
#include <gtest/gtest.h>
#include <cstdio>
#include <ctime>

#include "../binomial_heap.h"

/*
 * Integration test by heapsorting vector<int>
 */
TEST(IntegrationTest, TestByHeapsortOfInt){
    srand(time(0));
    int sz = rand() % 10000;
    std::vector<int> to_sort(sz);
    for (int i = 0; i < sz; i++)
        to_sort[i] = rand();
    std::vector<int> ans = to_sort;
    std::sort(ans.begin(), ans.end());
    BinomialHeap<int, std::less<int> > heap;
    for (int i = 0; i < sz; i++)
        heap.insert(to_sort[i]);
    for (int i = 0; i < sz; i++){
        ASSERT_EQ(heap.extractMin(), ans[i]);
    }
    ASSERT_TRUE(heap.empty());
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
