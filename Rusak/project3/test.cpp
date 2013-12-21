#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>

#include "AssignSumSegmentTree.h"

TEST(PrepareForTests, GenerateRand) {
  srand(time(NULL));
}

int rand_int() {
  return rand()%((int)(1e9));
}

TEST(UnitTests, AssignSumSmallData) {
  AssignSumSegmentTree<long long, 0> tree(0, 99);
  tree.assign(0, 99, 5);
  EXPECT_EQ(tree.sum(0, 99), 500);
  tree.assign(0, 39, 100);
  EXPECT_EQ(tree.sum(0, 99), 40*100+60*5);
  EXPECT_EQ(tree.sum(0, 9), 10*100);
  EXPECT_EQ(tree.sum(90, 99), 10*5);
  EXPECT_EQ(tree.sum(30, 49), 10*100+10*5);
}
