#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <climits>

#include "lib_advanced/GeneralSegmentTree.h"
#include "lib_advanced/AssignSumSegmentTree.h"
#include "lib_advanced/AssignAddMinMaxSumSegmentTree.h"
#include "lib_advanced/PermanentIntervalsSegmentTree.h"
#include "lib_advanced/VectorTree.h"

TEST(PrepareForTests, GenerateRand) {
  srand(time(NULL));
}

int rand_int() {
  return rand()%((int)(1e9));
}

typedef AssignSumSegmentTree<long long, 0> as;

typedef AssignAddMinMaxSumSegmentTree<long long, 0, LLONG_MAX/10, LLONG_MIN/10> aamms;
typedef aamms::ReturnType Return;

bool eq(const Return &a, const Return &b) {
  return (a.sum == b.sum && a.min == b.min && a.max == b.max);
}

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

TEST(StressTests, AssignAddMinMaxSumVectorTree) {
  const int count = 40000;
  VectorTree<long long, aamms::ReturnType> vt(count);
  aamms tree(0, count-1);
  tree.assign(0, count-1, 0);
  for (int i=0;i<count;i++) {
    int l, r, oper;
    oper = rand()%3;
    l = rand()%count;
    r = rand()%count;
    if (l > r) std::swap(l, r);
    if (oper==0) {
      int val = rand_int();
      tree.assign(l, r, val);
      vt.assign(l, r, val);
    }
    else if (oper==1) {
      int val = rand_int();
      tree.add(l, r, val);
      vt.add(l, r, val);
    }
    else {
      Return vtget = vt.get(l, r), treeget = tree.get(l, r);
      EXPECT_TRUE(eq(vtget, treeget));
    }
  }
}

TEST(StressTests, AssignSumVectorTree) {
  const int count = 60000;
  VectorTree<long long, aamms::ReturnType> vt(count);
  as tree(0, count-1);
  tree.assign(0, count-1, 0);
  for (int i=0;i<count;i++) {
    int l, r, oper;
    oper = rand()%2;
    l = rand()%count;
    r = rand()%count;
    if (l > r) std::swap(l, r);
    if (oper==0) {
      int val = rand_int();
      tree.assign(l, r, val);
      vt.assign(l, r, val);
    }
    else {
      Return vtget = vt.get(l, r);
      long long treeget = tree.sum(l, r);
      EXPECT_EQ(treeget, vtget.sum);
    }
  }
}

void speed_test_as(int count) {
  as tree(0, count-1);
  tree.assign(0, count-1, 0);
  for (int i=0;i<count;i++) {
    int l, r, oper;
    oper = rand()%2;
    l = rand()%count;
    r = rand()%count;
    if (l > r) std::swap(l, r);
    if (oper==0) {
      int val = rand_int();
      tree.assign(l, r, val);
    }
    else {
      long long treeget = tree.sum(l, r);
    }
  }
}

void speed_test_aamms(int count) {
  aamms tree(0, count-1);
  tree.assign(0, count-1, 0);
  for (int i=0;i<count;i++) {
    int l, r, oper;
    oper = rand()%3;
    l = rand()%count;
    r = rand()%count;
    if (l > r) std::swap(l, r);
    if (oper==0) {
      int val = rand_int();
      tree.assign(l, r, val);
    }
    else if (oper==1) {
      int val = rand_int();
      tree.add(l, r, val);
    }
    else {
      Return treeget = tree.get(l, r);
    }
  }
}

TEST(SpeedTests, As5000) {
  speed_test_as(5000);
}

TEST(SpeedTests, As50000) {
  speed_test_as(50000);
}

TEST(SpeedTests, As500000) {
  speed_test_as(500000);
}

TEST(SpeedTests, Aamms5000) {
  speed_test_aamms(5000);
}

TEST(SpeedTests, Aamms50000) {
  speed_test_aamms(50000);
}

TEST(SpeedTests, Aamms500000) {
  speed_test_aamms(500000);
}


