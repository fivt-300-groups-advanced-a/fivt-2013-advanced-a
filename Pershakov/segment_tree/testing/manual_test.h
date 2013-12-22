#ifndef MANUAL_TEST_H

#define MANUAL_TEST_H

#include <vector>
#include <functional>

#include "../cnt_equality_segments_tree.h"

TEST(ManualTest, CntEqualitySegments) {
    typedef CntEqualitySegmentsTree<int, std::less<int>, std::plus<int>, 0 > Tree;

    int sz = 8;
    std::vector<int> base(sz);
    for (int i = 0; i < sz; i++)
        base[i] = i + 1;

    Tree tree(base);

    // 1 2 3 4 5 6 7 8
    for (int i = 0; i < sz; i++)
        for (int j = i; j < sz; j++)
            ASSERT_EQ(tree.get(i, j), j - i + 1);
    
    // 3 4 5 6 7 8 9 10 
    tree.add(0, sz - 1, 2);
    for (int i = 0; i < sz; i++)
        for (int j = i; j < sz; j++)
            ASSERT_EQ(tree.get(i, j), j - i + 1);

    // 3 3 3 3 3 3 3 3
    tree.assign(0, sz - 1, 3);
    for (int i = 0; i < sz; i++)
        for (int j = i; j < sz; j++)
            ASSERT_EQ(tree.get(i, j), 1);

    // 1 2 3 4 5 6 7 8
    tree = Tree(base);

    // 2 3 4 5 5 6 7 8
    tree.add(0, 1, 1);
    ASSERT_EQ(tree.get(0, 7), 7);
    ASSERT_EQ(tree.get(3, 5), 3);

    // 2 5 5 5 5 6 7 8
    tree.assign(1, 3, 5);
    ASSERT_EQ(tree.get(0, 7), 5);
    ASSERT_EQ(tree.get(2, 4), 1);

    // 2 5 3 3 5 6 7 8
    tree.add(2, 3, -2);
    ASSERT_EQ(tree.get(0, 7), 7);
    ASSERT_EQ(tree.get(1, 6), 5);
}

TEST(ManualTest, MinMaxSumAssignmentAdd){
    typedef MinMaxSumAssignmentAddTree<int, std::less<int>, std::plus<int>,
            std::multiplies<int>, INT_MAX, INT_MIN, 0>
                Tree;

    int sz = 8;
    std::vector<int> base(sz);
    for (int i = 0; i < sz; i++)
        base[i] = sz - i;
    Tree tree(base);
    MinMaxSum<int> ans;

    // 8 7 6 5 4 3 2 1
    ans = MinMaxSum<int>(1, 8, 36);
    ASSERT_EQ(tree.get(0, 7), ans);
    ans = MinMaxSum<int>(4, 6, 15);
    ASSERT_EQ(tree.get(2, 4), ans);

    // 8 7 6 5 10 9 8 7
    tree.add(4, 7, 6);
    ans = MinMaxSum<int>(5, 10, 60);
    ASSERT_EQ(tree.get(0, 7), ans);
    ans = MinMaxSum<int>(7, 9, 24);
    ASSERT_EQ(tree.get(5, 7), ans);

    // 8 -2 -2 -2 -2 -2 8 7
    tree.assign(1, 5, -2);
    ans = MinMaxSum<int>(-2, 8, 0);
    ASSERT_EQ(tree.get(0, 4), ans);
    ans = MinMaxSum<int>(-2, -2, -2);
    ASSERT_EQ(tree.get(3, 3), ans);
}


TEST(ManualTest, MinMaxSumAssignment){
    typedef MinMaxSumAssignmentTree<int, std::less<int>, std::plus<int>,
            std::multiplies<int>, INT_MAX, INT_MIN, 0>
                Tree;

    int sz = 8;
    std::vector<int> base(sz);
    for (int i = 0; i < sz; i++)
        base[i] = i;
    Tree tree(base);
    MinMaxSum<int> ans;

    // 0 1 2 3 4 5 6 7
    ans = MinMaxSum<int>(0, 7, 28);
    ASSERT_EQ(tree.get(0, 7), ans);
    ans = MinMaxSum<int>(2, 4, 9);
    ASSERT_EQ(tree.get(2, 4), ans);

    // 0 1 2 3 6 6 6 6
    tree.assign(4, 7, 6);
    ans = MinMaxSum<int>(0, 6, 30);
    ASSERT_EQ(tree.get(0, 7), ans);
    ans = MinMaxSum<int>(6, 6, 18);
    ASSERT_EQ(tree.get(5, 7), ans);

    // 0 -2 -2 -2 -2 -2 6 6
    tree.assign(1, 5, -2);
    ans = MinMaxSum<int>(-2, 0, -8);
    ASSERT_EQ(tree.get(0, 4), ans);
    ans = MinMaxSum<int>(-2, -2, -2);
    ASSERT_EQ(tree.get(3, 3), ans);
}

TEST(ManualTest, MinMaxSumAdd){
    typedef MinMaxSumAddTree<int, std::less<int>, std::plus<int>,
            std::multiplies<int>, INT_MAX, INT_MIN, 0>
                Tree;

    int sz = 8;
    std::vector<int> base(sz);
    for (int i = 0; i < sz; i++)
        base[i] = i;
    Tree tree(base);
    MinMaxSum<int> ans;

    // 0 1 2 3 4 5 6 7
    ans = MinMaxSum<int>(0, 7, 28);
    ASSERT_EQ(tree.get(0, 7), ans);
    ans = MinMaxSum<int>(2, 4, 9);
    ASSERT_EQ(tree.get(2, 4), ans);

    // 0 1 2 3 6 7 8 9
    tree.add(4, 7, 2);
    ans = MinMaxSum<int>(0, 9, 36);
    ASSERT_EQ(tree.get(0, 7), ans);
    ans = MinMaxSum<int>(7, 9, 24);
    ASSERT_EQ(tree.get(5, 7), ans);

    // 0 0 1 2 5 6 8 9
    tree.add(1, 5, -1);
    ans = MinMaxSum<int>(0, 5, 8);
    ASSERT_EQ(tree.get(0, 4), ans);
    ans = MinMaxSum<int>(2, 2, 2);
    ASSERT_EQ(tree.get(3, 3), ans);
}

TEST(ManualTest, MaxSumSegment) {
    typedef MaxSumSegmentTree<int, std::less<int>, std::plus<int>,
            std::multiplies<int>, 0>
                Tree;

    int sz = 8;
    std::vector<int> base(sz);
    for (int i = 0; i < sz; i++)
        base[i] = i;
    Tree tree(base);
    Segment<int> ans;

    // 0 1 2 3 4 5 6 7
    ans = tree.get(0, 7);
    ASSERT_EQ(ans.sum, 28);
    ASSERT_TRUE(ans.left == 0 || ans.left == 1);
    ASSERT_EQ(ans.right, 7);

    ans = tree.get(1, 2);
    ASSERT_EQ(ans.sum, 3);
    ASSERT_EQ(ans.left, 1);
    ASSERT_EQ(ans.right, 2);

    // -2 -2 -2 -2 -2 5 6 7
    tree.assign(0, 4, -2);
    ans = tree.get(0, 7);
    ASSERT_EQ(ans.sum, 18);
    ASSERT_EQ(ans.left, 5);
    ASSERT_EQ(ans.right, 7);

    ans = tree.get(0, 4);
    ASSERT_EQ(ans.sum, -2);
    ASSERT_EQ(ans.left, ans.right);
    ASSERT_TRUE(0 <= ans.left && ans.left <= 4); 

    // -2 -2 3 3 -2 5 6 7
    tree.assign(2, 3, 3);
    ans = tree.get(0, 7);
    ASSERT_EQ(ans.sum, 22);
    ASSERT_EQ(ans.left, 2);
    ASSERT_EQ(ans.right, 7);

    ans = tree.get(0, 4);
    ASSERT_EQ(ans.sum, 6);
    ASSERT_EQ(ans.left, 2);
    ASSERT_EQ(ans.right, 3);
}


#endif
