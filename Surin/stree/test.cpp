#include <gtest/gtest.h>
#include <cstdio>
#include <utility>
#include <list>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <vector>
#include <algorithm>
#include <ctime>
#include "trees.h"
#include <iostream>

TEST(HandMade, AssMinTree) {
    AssMinTree<int, (int)1e+9> s(0, 4);
    s.push(0, 4, 1);
    EXPECT_EQ(s.fold(2, 4), 1);
    s.push(0, 4, 0);
    EXPECT_EQ(s.fold(3, 4), 0);
    s.push(2, 4, 8);
    s.push(3, 4, 9);
    EXPECT_EQ(s.fold(2, 4), 8);
    std::cerr << "-----\n";
    s.push(0, 1, 7);
    EXPECT_EQ(s.fold(0, 1), 7);
}

TEST(Stress, AssMinTree) {
    std::vector<int> v(10, RAND_MAX);
    AssMinTree<int, RAND_MAX> s(0, 10);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(9 - l) + l + 1;
        int ss = rand();
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] = ss;
        } else {
            int mx = RAND_MAX;
            for(int i = l; i < r; i++)
                mx = std::min(mx, v[i]);
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}


TEST(Stress, AssMaxTree) {
    std::vector<int> v(10, -RAND_MAX);
    AssMaxTree<int, -RAND_MAX> s(0, 10);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(9 - l) + l + 1;
        int ss = rand();
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] = ss;
        } else {
            int mx = -RAND_MAX;
            for(int i = l; i < r; i++)
                mx = std::max(mx, v[i]);
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}


TEST(Stress, AssSumTree) {
    std::vector<int> v(10, 0);
    AssSumTree<int, 0> s(0, 10);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(9 - l) + l + 1;
        int ss = rand()%20;
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] = ss;
        } else {
            int mx = 0;
            for(int i = l; i < r; i++)
                mx = mx + v[i];
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}

TEST(Stress, AddMinTree) {
    std::vector<int> v(10, 0);
    AddMinTree<int, 0, RAND_MAX> s(0, 10);
    s.push(0, 10, -RAND_MAX);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(9 - l) + l + 1;
        int ss = rand()%20;
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] += ss;
        } else {
            int mx = RAND_MAX;
            for(int i = l; i < r; i++)
                mx = std::min(mx, v[i]);
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}

TEST(Stress, AddMaxTree) {
    std::vector<int> v(10, 0);
    AddMaxTree<int, 0, -RAND_MAX> s(0, 10);
    s.push(0, 10, RAND_MAX);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(9 - l) + l + 1;
        int ss = rand()%20;
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] += ss;
        } else {
            int mx = -RAND_MAX;
            for(int i = l; i < r; i++)
                mx = std::max(mx, v[i]);
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}

TEST(Stress, AddSumTree) {
    std::vector<int> v(10, 0);
    AddSumTree<int, 0> s(0, 10);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(9 - l) + l + 1;
        int ss = rand()%20;
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] += ss;
        } else {
            int mx = 0;
            for(int i = l; i < r; i++)
                mx = mx + v[i];
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}


TEST(Stress, Internal) {
    
}

int main(int argc, char ** argv) { //1231
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
