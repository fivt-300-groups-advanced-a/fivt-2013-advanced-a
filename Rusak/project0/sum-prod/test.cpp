#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <vector>
#include<cassert>

#include "stupid.h"
#include "clever.h"

TEST(PrepareForTests, GenerateRand) {
  srand(time(NULL));
}

int rand10() {
  return (rand()%(21))-10;
}

int rand1() {
  return (rand()%3)-1;
}

TEST(StressTest, Main) {
  int col = 100000;
  for (int te=0;te<col;te++) {
    int sz = rand()%9+1;
    std::vector<int> data;
    data.resize(sz);
    for (int i=0;i<sz;i++) data[i] = rand10();
    Clever cl(data);
    Stupid st(data);
    EXPECT_EQ(cl.solve(), st.solve());
  }
}

void speed_test_stupid(int col) {
  std::vector<int> data;
  for (int i=0;i<col;i++) data.push_back(rand1());
  Stupid st(data);
  long long res = st.solve();
  assert(res>-(int)(2e9)); 
}

void speed_test_clever(int col) {
  std::vector<int> data;
  for (int i=0;i<col;i++) data.push_back(rand1());
  Clever cl(data);
  long long res = cl.solve();
  assert(res>-(int)(2e9)); 
}

TEST(SpeedTests, Stupid20) {
  speed_test_stupid(20);
}

TEST(SpeedTests, Stupid25) {
  speed_test_stupid(25);
}

TEST(SpeedTests, Clever1000) {
  speed_test_clever(1000);
}

TEST(SpeedTests, Clever15000) {
  speed_test_clever(15000);
}


