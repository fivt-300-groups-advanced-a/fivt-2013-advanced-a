#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <climits>

#include "lib_advanced/GeneralSegmentTree.h"
#include "lib_advanced/AssignSumSegmentTree.h"
#include "lib_advanced/AssignAddMinMaxSumSegmentTree.h"

TEST(PrepareForTests, GenerateRand) {
  srand(time(NULL));
}

int rand_int() {
  return rand()%((int)(1e9));
}

typedef AssignSumSegmentTree<long long, 0> as;

typedef AssignAddMinMaxSumSegmentTree<long long, 0, LLONG_MAX, LLONG_MIN> aamms;

TEST(UnitTests, AssignSumSmallData) {
  as tree(0, 99);
  tree.assign(0, 99, 5);
  EXPECT_EQ(tree.sum(0, 99), 500);
  tree.assign(0, 39, 100);
  EXPECT_EQ(tree.sum(0, 99), 40*100+60*5);
  EXPECT_EQ(tree.sum(0, 9), 10*100);
  EXPECT_EQ(tree.sum(90, 99), 10*5);
  EXPECT_EQ(tree.sum(30, 49), 10*100+10*5);
}

TEST(UnitTests, AssignAddMinMaxSumSmallData) {
  aamms tree(0, 99);
  tree.assign(0, 99, 5);

  auto ret = tree.get(0, 99);
  EXPECT_EQ(ret.sum, 500);
  EXPECT_EQ(ret.max, 5);
  EXPECT_EQ(ret.min, 5);
  tree.assign(0, 39, 100);

  ret = tree.get(0, 99);
  EXPECT_EQ(ret.sum, 40*100+60*5);
  EXPECT_EQ(ret.max, 100);
  EXPECT_EQ(ret.min, 5);

  ret = tree.get(0, 9);
  EXPECT_EQ(ret.sum, 10*100);
  EXPECT_EQ(ret.min, 100);
  EXPECT_EQ(ret.max, 100);

  ret = tree.get(90, 99);
  EXPECT_EQ(ret.sum, 10*5);
  EXPECT_EQ(ret.min, 5);
  EXPECT_EQ(ret.max, 5);

  ret = tree.get(30, 49);
  EXPECT_EQ(ret.sum, 10*100+10*5);
  EXPECT_EQ(ret.min, 5);
  EXPECT_EQ(ret.max, 100);
}
