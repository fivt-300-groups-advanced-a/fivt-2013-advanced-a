#ifndef STRESS_TEST_H

#define STRESS_TEST_H

#include <vector>
#include <algorithm>
#include <functional>
#include <climits>

#include "testing_utilities.h"
#include "../cnt_equality_segments_tree.h"
#include "../min_max_sum_tree.h"
#include "../max_sum_segment_tree.h"

TEST(StressTest, CntEqualitySegments) {
    typedef CntEqualitySegmentsTree<int, std::less<int>, std::plus<int>, 0> Tree;

    testing_utilities::testAssignmentAddTree
        <Tree, testing_utilities::GetCntEqualitySegments>();
}

TEST(StressTest, MinMaxSumAssignmentAdd) {
    typedef MinMaxSumAssignmentAddTree<int, std::less<int>, std::plus<int>, 
            std::multiplies<int>, INT_MAX, INT_MIN, 0>
                Tree;

    testing_utilities::testAssignmentAddTree
        <Tree, testing_utilities::GetMinMaxSum>();
}

TEST(StressTest, MinMaxSumAssignment) {
    typedef MinMaxSumAssignmentTree<int, std::less<int>, std::plus<int>, 
            std::multiplies<int>, INT_MAX, INT_MIN, 0>
                Tree;

    testing_utilities::testAssignmentTree
        <Tree, testing_utilities::GetMinMaxSum>();
}

TEST(StressTest, MinMaxSumAdd) {
    typedef MinMaxSumAddTree<int, std::less<int>, std::plus<int>,
            std::multiplies<int>, INT_MAX, INT_MIN, 0>
                Tree;

    testing_utilities::testAddTree
        <Tree, testing_utilities::GetMinMaxSum>();
}

bool checkAnswer(const std::vector<int> &slow_tree, int l, int r, 
        const Segment<int> &ans) {
    int sum = 0;
    for (int i = ans.left; i <= ans.right; i++)
        sum += slow_tree[i];
    if (sum != ans.sum)
        return false;
    for (int i = l; i <= r; i++){
        int cur_sum = 0;
        for (int j = i; j <= r; j++){
            cur_sum += slow_tree[j];
            if (cur_sum > sum)
                return false;
        }
    }
    return true;
}

TEST(StressTest, MaxSumSegment) {
    typedef MaxSumSegmentTree<int, std::less<int>, std::plus<int>,
            std::multiplies<int>, 0> 
                Tree;

    Tree tree;
    std::vector<int> slow_tree;
    testing_utilities::initIntTrees(slow_tree, tree);
    int sz = slow_tree.size();
    int cnt_queries = rand() % 1000;
    for(int i = 0; i < cnt_queries; i++) {
        int t = rand() % 2;
        int l = rand() % sz;
        int r = rand() % sz;
        if (l > r)
            std::swap(l, r);
        if (t == 0) {
            ASSERT_TRUE(checkAnswer(slow_tree, l, r, tree.get(l, r)));
        } else {
            int val = testing_utilities::randomInt();
            tree.assign(l, r, val);
            testing_utilities::assign(slow_tree, l, r, val);
        }

    }
}

#endif
