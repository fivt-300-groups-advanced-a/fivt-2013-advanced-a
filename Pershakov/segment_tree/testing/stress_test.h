#ifndef STRESS_TEST_H

#define STRESS_TEST_H

#include <vector>
#include <algorithm>

#include "../sum_segment_tree.h"
#include "../min_segment_tree.h"
#include "../max_segment_tree.h"
#include "testing_utilities.h"

TEST(StressTest, SumTreeAddWithVector) {
    typedef SumTreeAdd Tree;
    typedef SumSegmentTreeAdd::TreeNode TreeNode;
    typedef SumSegmentTreeAdd::UpdInfo UpdInfo;
    
    testing_utilities::testAddTree
        <Tree, TreeNode, UpdInfo, testing_utilities::Sum>();
}

TEST(StressTest, SumTreeAssignmentWithVector) {
    typedef SumTreeAssignment Tree;
    typedef SumSegmentTreeAssignment::TreeNode TreeNode;
    typedef SumSegmentTreeAssignment::UpdInfo UpdInfo;

    testing_utilities::testAssignmentTree
        <Tree, TreeNode, UpdInfo, testing_utilities::Sum>();
}

TEST(StressTest, SumTreeAssignmentAddWithVector) {
    typedef SumTreeAssignmentAdd Tree;
    typedef SumSegmentTreeAssignmentAdd::TreeNode TreeNode;
    typedef SumSegmentTreeAssignmentAdd::UpdInfo UpdInfo;

    testing_utilities::testAssignmentAddTree
        <Tree, TreeNode, UpdInfo, testing_utilities::Sum>();
}

TEST(StressTest, MinTreeAddWithVector) {
    typedef MinTreeAdd Tree;
    typedef MinSegmentTreeAdd::TreeNode TreeNode;
    typedef MinSegmentTreeAdd::UpdInfo UpdInfo;
    
    testing_utilities::testAddTree
        <Tree, TreeNode, UpdInfo, testing_utilities::Min>();
}

TEST(StressTest, MinTreeAssignmentWithVector) {
    typedef MinTreeAssignment Tree;
    typedef MinSegmentTreeAssignment::TreeNode TreeNode;
    typedef MinSegmentTreeAssignment::UpdInfo UpdInfo;
    
    testing_utilities::testAssignmentTree
        <Tree, TreeNode, UpdInfo, testing_utilities::Min>();
}

TEST(StressTest, MinTreeAssignmentAddWithVector) {
    typedef MinTreeAssignmentAdd Tree;
    typedef MinSegmentTreeAssignmentAdd::TreeNode TreeNode;
    typedef MinSegmentTreeAssignmentAdd::UpdInfo UpdInfo;
    
    testing_utilities::testAssignmentAddTree
        <Tree, TreeNode, UpdInfo, testing_utilities::Min>();
}

TEST(StressTest, MaxTreeAddWithVector) {
    typedef MaxTreeAdd Tree;
    typedef MaxSegmentTreeAdd::TreeNode TreeNode;
    typedef MaxSegmentTreeAdd::UpdInfo UpdInfo;
    
    testing_utilities::testAddTree
        <Tree, TreeNode, UpdInfo, testing_utilities::Max>();
}

TEST(StressTest, MaxTreeAssignmentWithVector) {
    typedef MaxTreeAssignment Tree;
    typedef MaxSegmentTreeAssignment::TreeNode TreeNode;
    typedef MaxSegmentTreeAssignment::UpdInfo UpdInfo;
    
    testing_utilities::testAssignmentTree
        <Tree, TreeNode, UpdInfo, testing_utilities::Max>();
}

TEST(StressTest, MaxTreeAssignmentAddWithVector) {
    typedef MaxTreeAssignmentAdd Tree;
    typedef MaxSegmentTreeAssignmentAdd::TreeNode TreeNode;
    typedef MaxSegmentTreeAssignmentAdd::UpdInfo UpdInfo;
    
    testing_utilities::testAssignmentAddTree
        <Tree, TreeNode, UpdInfo, testing_utilities::Max>();
}

#endif
