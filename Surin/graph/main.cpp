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

const int N = 200;

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
    for (auto i = g.getIncedents(1)->begin(); !i->end(); ++(*i)) {
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
    for (auto i = g.getIncedents(1)->begin(); !i->end(); ++(*i)) {
        s.insert(**i);
    }
    EXPECT_EQ(s.size(), 2);
    EXPECT_NE(s.end(), s.find(0));
    EXPECT_NE(s.end(), s.find(2));
}

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
                    return new VectorIntList(std::move(v));
                }
            ));
    for (int i = 0; i < N; i++) {
        std::vector<int> vv;
        for (auto j = g.getIncedents(i)->begin(); !j->end(); ++(*j)) {
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
                    return new VectorBoolList(std::move(v));
                }
            ));
    for (int i = 0; i < N; i++) {
        std::vector<int> vv;
        int kolit = 0, kolG = 0;
        for (int j = 0; j < N; j++)
            kolG += G[i][j];
        for (auto j = g.getIncedents(i)->begin(); !j->end(); ++(*j), kolit++) {
            EXPECT_TRUE(G[i][**j]);
        }
        EXPECT_EQ(kolit, kolG);
        for (int j = 0; j < N; j++) {
            EXPECT_EQ(g.isConnected(i, j), G[i][j]);
        }
    }
}


TEST(Random, OneList) {
    std::vector<int> G[N];
    Graph g(map(N,
                [=,&G](int x) {
                    int cc = rand() % N;
                    std::vector<int> v;
                    int c = rand() % N;
                    v.push_back(c);
                    std::sort(v.begin(), v.end());
                    v.erase(std::unique(v.begin(), v.end()), v.end());
                    std::random_shuffle(v.begin(), v.end());
                    std::copy(v.begin(), v.end(), std::back_inserter<std::vector<int> >(G[x]));
                    return new OneList(c);
                }
            ));
    for (int i = 0; i < N; i++) {
        std::vector<int> vv;
        for (auto j = g.getIncedents(i)->begin(); !j->end(); ++(*j)) {
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

std::vector<bool> mark;

bool find(int u, int v, const Graph & g) {
    if (mark[u]) return false;
    mark[u] = true;
    if (u == v) return true;
    for (auto i = g.getIncedents(u)->begin(); !i->end(); ++*i) {
        if (find(**i, v, g)) return true;
    }
    return false;
}

bool isCon(int u, int v, const Graph & g) {
    mark.assign(g.size(), false);
    return find(u, v, g);
}

TEST(Algorithm, StrongComps) {
    std::vector<std::vector<int> > G(N), Gtmp;
    for (int i = 0; i < N; i++) {
        int Nn = rand() % (N / 2);
        for (int cc = 0; i < Nn; i++)
            G[i].push_back(rand() % N);
    }
    Gtmp = G;
    Graph GG(map(N, [&](int i) {
                    return new VectorIntList(std::move(Gtmp[i])); 
                }));
    std::vector<int> vv = getStrongComponents(GG);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            EXPECT_EQ(vv[i] == vv[j], isCon(i, j, GG) && isCon(j, i, GG)) << " " << i << " " << j;
        }
    }
}


TEST(Algorithm, addEdges) {
    std::vector<std::vector<int> > G(N), Gtmp;
    for (int i = 0; i < N; i++) {
        int Nn = rand() % (N);
        for (int cc = 0; i < Nn; i++) {
            int tt  = rand() % N;
            G[i].push_back(tt);
        }
    }
    Gtmp = G;
    Graph G1(map(N, [&](int i) {
                    return new VectorIntList(std::move(Gtmp[i])); 
                }));
    std::vector<std::pair<int, int> > edg = addEdges(G1);
    for (int i = 0; i < edg.size(); i++) {
        G[edg[i].first].push_back(edg[i].second);
    }
    Graph G2(map(N, [&](int i) {
                    return new VectorIntList(std::move(G[i])); 
                }));
    std::vector<int> sG = getStrongComponents(G2);
    for (int i = 0; i < N; i++)
        EXPECT_EQ(sG[i], 1);
}
