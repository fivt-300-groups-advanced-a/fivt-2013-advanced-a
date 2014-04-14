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
#include <iostream>
#include <set>
#include "graph.h"
#include "lists_of_incidents.h"

TEST(handmade, vectorInt) { 
    Graph g(map(4, 
                [=](int x) {
                    if (x == 0) 
                        return new VectorIntList(std::vector<int>({1, 2}));
                    if (x == 1)
                        return new VectorIntList(std::vector<int>({0, 2}));
                    return new VectorIntList(std::vector<int>({0}));
                }
            ));
    EXPECT_TRUE(g.isConnected(0, 1));
    EXPECT_TRUE(g.isConnected(1, 2));
    EXPECT_FALSE(g.isConnected(3, 1));
    EXPECT_TRUE(g.isConnected(2, 0));
    
    std::set<int> s;
    for (auto i = g.getIncidents(1)->begin(); !i->end(); ++(*i)) {
        s.insert(**i);
    }
    EXPECT_EQ(s.size(), 2);
    EXPECT_NE(s.end(), s.find(0));
    EXPECT_NE(s.end(), s.find(2));
}

TEST(handmade, vectorBool) { 
    Graph g(map(4, 
                [=](int x) {
                    if (x == 0) 
                        return new VectorBoolList(std::vector<bool>({0, 1, 1, 0}));
                    if (x == 1)
                        return new VectorBoolList(std::vector<bool>({1, 0, 1, 0}));
                    return new VectorBoolList(std::vector<bool>({1, 0, 0, 0}));
                }
            ));
    EXPECT_TRUE(g.isConnected(0, 1));
    EXPECT_TRUE(g.isConnected(1, 2));
    EXPECT_FALSE(g.isConnected(3, 1));
    EXPECT_TRUE(g.isConnected(2, 0));
    
    std::set<int> s;
    for (auto i = g.getIncidents(1)->begin(); !i->end(); ++(*i)) {
        s.insert(**i);
    }
    EXPECT_EQ(s.size(), 2);
    EXPECT_NE(s.end(), s.find(0));
    EXPECT_NE(s.end(), s.find(2));
}

const int N = 20;

TEST(Random, vectorInt) {
    std::vector<int> G[N];
    Graph g(map(N,
                [=,&G](int x) {
                    int cc = rand() % N;
                    std::vector<int> v;
                    for (int i = 0; i < cc; i++) {
                        v.push_back(rand() % N);
                    }
                    std::sort(v.begin(), v.end());
                    v.erase(std::unique(v.begin(), v.end()), v.end());
                    std::random_shuffle(v.begin(), v.end());
                    std::copy(v.begin(), v.end(), std::back_inserter<std::vector<int> >(G[x]));
                    return new VectorIntList(v);
                }
            ));
    for (int i = 0; i < N; i++) {
        std::vector<int> vv;
        for (auto j = g.getIncidents(i)->begin(); !j->end(); ++(*j)) {
            vv.push_back(**j);
        }
        std::sort(vv.begin(), vv.end());
        std::sort(G[i].begin(), G[i].end());
        EXPECT_TRUE(vv == G[i]);
        for (int j = 0; j < N; j++) {
            EXPECT_EQ(g.isConnected(i, j), std::find(vv.begin(), vv.end(), j) != vv.end());
        }
    }
}


TEST(Random, vectorBool) {
    std::vector<bool> G[N];
    Graph g(map(N,
                [=,&G](int x) {
                    int cc = rand() % N;
                    G[x].resize(N);
                    std::vector<bool> v(N);
                    std::fill(v.begin(), v.end(), false);
                    for (int i = 0; i < N; i++) {
                        if (rand() % 2) {
                            v[i] = true;
                            G[x][i] = true;
                        }
                    }
                    return new VectorBoolList(v);
                }
            ));
    for (int i = 0; i < N; i++) {
        std::vector<int> vv;
        int kolit = 0, kolG = 0;
        for (int j = 0; j < N; j++)
            kolG += G[i][j];
        for (auto j = g.getIncidents(i)->begin(); !j->end(); ++(*j), kolit++) {
            EXPECT_TRUE(G[i][**j]);
        }
        EXPECT_EQ(kolit, kolG);
        for (int j = 0; j < N; j++) {
            EXPECT_EQ(g.isConnected(i, j), G[i][j]);
        }
    }
}
