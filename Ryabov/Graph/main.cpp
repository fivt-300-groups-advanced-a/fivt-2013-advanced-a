#define TESTING
#include <iostream>
#include <queue>
#include <gtest/gtest.h>
#include <utility>
#include <ctime>
#include <map>
#include "graph.h"
#include <algorithm>
#include "algo.h"

using namespace std;


TEST(OneIncidents, Hand1)
{
    int n = 3;
    vector<unique_ptr<Incidents>> v(n);
    vector<OneFactory> lf(n);
    for (int i = 0; i < n; ++i)
    {
        lf[i].addEdge((i + 1) % n);
    }
    for (int i = 0; i < n; ++i)
        v[i] = lf[i].get();
    Graph g(v);
    int k;
    for (int i = 0; i < n; ++i)
    {
        auto it = g[i].begin();
        while (it->get() != -1)
        {
            it->next();
            k++;
        }
    }
    EXPECT_EQ(g[0].isConnected(1), true);
    EXPECT_EQ(g[1].isConnected(2), true);
    EXPECT_EQ(g[2].isConnected(0), true);
    EXPECT_EQ(g[0].isConnected(2), false);
    EXPECT_EQ(g[0].isConnected(0), false);
    EXPECT_EQ(g[1].isConnected(0), false);
}


TEST(ListIncidents, Hand1)
{
    int n = 3;
    vector<unique_ptr<Incidents>> v(n);
    vector<ListFactory> lf(n);
    for (int i = 0; i < n; ++i)
    {
        lf[i].addEdge((i + 1) % n);
    }
    for (int i = 0; i < n; ++i)
        v[i] = lf[i].get();
    Graph g(v);
    int k;
    for (int i = 0; i < n; ++i)
    {
        auto it = g[i].begin();
        while (it->get() != -1)
        {
            it->next();
            k++;
        }
    }
    EXPECT_EQ(g[0].isConnected(1), true);
    EXPECT_EQ(g[1].isConnected(2), true);
    EXPECT_EQ(g[2].isConnected(0), true);
    EXPECT_EQ(g[0].isConnected(2), false);
    EXPECT_EQ(g[0].isConnected(0), false);
    EXPECT_EQ(g[1].isConnected(0), false);
}


TEST(BitsetIncidents, Hand1)
{
    int n = 3;
    vector<unique_ptr<Incidents>> v(n);
    vector<BitsetFactory> lf(n, BitsetFactory(n));
    for (int i = 0; i < n; ++i)
    {
        lf[i].addEdge((i + 1) % n);
    }
    for (int i = 0; i < n; ++i)
        v[i] = lf[i].get();
    Graph g(v);
    int k;
    for (int i = 0; i < n; ++i)
    {
        auto it = g[i].begin();
        while (it->get() != -1)
        {
            it->next();
            k++;
        }
    }
    EXPECT_EQ(g[0].isConnected(1), true);
    EXPECT_EQ(g[1].isConnected(2), true);
    EXPECT_EQ(g[2].isConnected(0), true);
    EXPECT_EQ(g[0].isConnected(2), false);
    EXPECT_EQ(g[0].isConnected(0), false);
    EXPECT_EQ(g[1].isConnected(0), false);
}


TEST(ListIncidents, Stress1)
{
    int n = 1000;
    int m = 10000;
    vector<unique_ptr<Incidents>> v(n);
    vector<ListFactory> lf(n);
    vector<vector<int>> mg(n);
    for (int i = 0; i < m; ++i)
    {
        int q = rand() % n;
        int w = rand() % n;
        lf[q].addEdge(w);
        mg[q].push_back(w);
    }
    for (int i = 0; i < n; ++i)
    {
        v[i] = lf[i].get();
        sort(mg[i].begin(), mg[i].end());
    }
    Graph g(v);
    int k = 0;
    for (int i = 0; i < n; ++i)
    {
        auto it = g[i].begin();
        auto mit = mg[i].begin();
        while (it->get() != -1)
        {
            EXPECT_EQ(it->get(), *mit);
            mit++;
            it->next();
            k++;
        }
        EXPECT_EQ(mit, mg[i].end());
    }
    EXPECT_EQ(k, m);
}

TEST (Tarjan, Hand1)
{
    int n = 100;
    int k = 5;
    vector<unique_ptr<Incidents>> v(n);
    vector<ListFactory> lf(n);
    for (int i = 0; i < n; i += k)
    {
        for (int j = 1; j < k; ++j)
            if (i + j < n)
            {
                lf[i].addEdge(i + j);
                lf[i + j].addEdge(i);
            }
    }
    for (int i = 0; i < n; ++i)
        v[i] = lf[i].get();
    Graph g(v);
    vector<int> tmp;
    EXPECT_EQ(getStrongCon(g, tmp), n / k);
}

TEST (Tarjan, Hand2)
{
    int n = 100;
    int k = 5;
    vector<unique_ptr<Incidents>> v(n);
    vector<ListFactory> lf(n);
    for (int i = 0; i < n; i += k)
    {
        if (i != 0)
            lf[i].addEdge(i - 1);
        for (int j = 1; j < k; ++j)
            if (i + j < n)
            {
                lf[i].addEdge(i + j);
                lf[i + j].addEdge(i);
            }
    }
    for (int i = 0; i < n; ++i)
        v[i] = lf[i].get();
    Graph g(v);
    vector<int> tmp;
    EXPECT_EQ(getStrongCon(g, tmp), n / k);
}

TEST (ALGO, Stress1)
{
    for (int qwe = 0; qwe < 100; qwe++)
    {
        int n = 1000;
        int m = 1000;
        vector<unique_ptr<Incidents>> v(n);
        vector<ListFactory> lf(n);
        for (int i = 0; i < m; ++i)
        {
            int q = 0, w = 0;
            while (q != w)
            {
                q = rand() % n;
                w = rand() % n;
            }
            lf[q].addEdge(w);
        }
        for (int i = 0; i < n; ++i)
            v[i] = lf[i].get();
        Graph g(v);
        vector<pair<int, int>> res = StrongConnectivityAugmentation(g);
        for (int i = 0; i < res.size(); ++i)
            lf[res[i].first].addEdge(res[i].second);
        for (int i = 0; i < n; ++i)
            v[i] = lf[i].get();
        Graph f(v);
        vector<int> tmp;
        EXPECT_EQ(1, getStrongCon(f, tmp));
    }
}
TEST (ALGO, Stress2)
{
    for (int qwe = 0; qwe < 100; qwe++)
    {
        int n = 100;
        int m = 1000;
        vector<unique_ptr<Incidents>> v(n);
        vector<ListFactory> lf(n);
        for (int i = 0; i < m; ++i)
        {
            int q = 0, w = 0;
            while (q != w)
            {
                q = rand() % n;
                w = rand() % n;
            }
            lf[q].addEdge(w);
        }
        for (int i = 0; i < n; ++i)
            v[i] = lf[i].get();
        Graph g(v);
        vector<pair<int, int>> res = StrongConnectivityAugmentation(g);
        for (int i = 0; i < res.size(); ++i)
            lf[res[i].first].addEdge(res[i].second);
        for (int i = 0; i < n; ++i)
            v[i] = lf[i].get();
        Graph f(v);
        vector<int> tmp;
        EXPECT_EQ(1, getStrongCon(f, tmp));
    }
}

TEST(BitsetIncidents, Stress1)
{
    int n = 1000;
    int m = 100000;
    vector<unique_ptr<Incidents>> v(n);
    vector<BitsetFactory> lf(n, BitsetFactory(n));
    vector<vector<int>> mg(n);
    for (int i = 0; i < m; ++i)
    {
        int q = rand() % n;
        int w = rand() % n;
        lf[q].addEdge(w);
        mg[q].push_back(w);
    }
    for (int i = 0; i < n; ++i)
    {
        v[i] = lf[i].get();
        sort(mg[i].begin(), mg[i].end());
        mg[i].resize(unique(mg[i].begin(), mg[i].end()) - mg[i].begin());
    }
    Graph g(v);
    int k = 0;
    for (int i = 0; i < n; ++i)
    {
        auto it = g[i].begin();
        auto mit = mg[i].begin();
        while (it->get() != -1)
        {
            EXPECT_EQ(it->get(), *mit);
            mit++;
            it->next();
            k++;
        }
        EXPECT_EQ(mit, mg[i].end());
    }
}


TEST(OneIncidents, Stress1)
{
    int n = 1000;
    int m = 1000;
    vector<unique_ptr<Incidents>> v(n);
    vector<OneFactory> lf(n);
    vector<vector<int>> mg(n);
    for (int i = 0; i < m; ++i)
    {
        int q = rand() % n;
        int w = rand() % n;
        lf[q].addEdge(w);
        if (mg[q].size() == 1)
            mg[q][0] = w;
        else
            mg[q].push_back(w);
    }
    for (int i = 0; i < n; ++i)
    {
        v[i] = lf[i].get();
        sort(mg[i].begin(), mg[i].end());
        mg[i].resize(unique(mg[i].begin(), mg[i].end()) - mg[i].begin());
    }
    Graph g(v);
    int k = 0;
    for (int i = 0; i < n; ++i)
    {
        auto it = g[i].begin();
        auto mit = mg[i].begin();
        while (it->get() != -1)
        {
            EXPECT_EQ(it->get(), *mit);
            mit++;
            it->next();
            k++;
        }
        EXPECT_EQ(mit, mg[i].end());
    }
}


TEST(CleverIncidents, Stress1)
{
    int n = 1000;
    int m = 1000000;
    vector<unique_ptr<Incidents>> v(n);
    vector<CleverFactory> lf(n, CleverFactory(n));
    vector<vector<int>> mg(n);
    for (int i = 0; i < m; ++i)
    {
        int q = rand() % n;
        int w = rand() % n;
        lf[q].addEdge(w);
        mg[q].push_back(w);
    }
    for (int i = 0; i < n; ++i)
    {
        v[i] = lf[i].get();
        sort(mg[i].begin(), mg[i].end());
        mg[i].resize(unique(mg[i].begin(), mg[i].end()) - mg[i].begin());
    }
    Graph g(v);
    int k = 0;
    for (int i = 0; i < n; ++i)
    {
        auto it = g[i].begin();
        auto mit = mg[i].begin();
        while (it->get() != -1)
        {
            EXPECT_EQ(it->get(), *mit);
            mit++;
            it->next();
            k++;
        }
        EXPECT_EQ(mit, mg[i].end());
    }
}


int main(int argc, char **argv)
{
    srand(0);
    testing :: InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
