#ifndef STRONG_CONNECTIVITY_TESTING_H

#define STRONG_CONNECTIVITY_TESTING_H

#include <vector>

#include "testing_utilities.h"

#include "../factory.h"
#include "../graph.h"
#include "../problems/strong_connectivity.h"

void dfs(const Graph &gr, int v, std::vector<bool> &used) {
    const ListOfIncidents& li = gr.getIncidents(v);
    used[v] = true;

    std::unique_ptr<Iterator> it = std::move(li.getIterator());
    for (; it->getVertex() != -1; it->next()) {
        int to = it->getVertex();
        if (!used[to])
            dfs(gr, to, used);
    }
}

void check_components(const Graph &gr, const std::vector<int> &ans) {
    int sz = ans.size();
    std::vector<std::vector<bool> > conn(sz);
    for (int i = 0; i < sz; i++) {
        conn[i].resize(sz);
        dfs(gr, i, conn[i]);
    }

    for (int i = 0; i < sz; i++)
        for (int j = 0; j < sz; j++)
            if (ans[i] == ans[j])
                ASSERT_TRUE(conn[i][j] && conn[j][i]);
            else
                ASSERT_FALSE(conn[i][j] && conn[j][i]);
}

TEST(StrongConnectivityTesting, StronglyConnectedComponentsTesting) {
    int cnt_tests = 100;
    for (int i = 0; i < cnt_tests; i++) {
        Graph gr;
        int sz = rand() % 200;
        buildRandomGraph(gr, sz);

        StrongConnectivity solver;        
        std::vector<int> ans = solver.getStronglyConnectedComponents(gr);
        ASSERT_EQ(sz, ans.size());
        
        check_components(gr, ans);
    }
}

TEST(CondensationTesting, BambooTest) {
    int sz = 10;
    std::vector<std::unique_ptr<ListOfIncidents> > g(sz);
    for (int i = 0; i < sz - 1; i++) {
        g[i] = std::unique_ptr<ListOfIncidents>
            (new FunctionalListOfIncidents(i + 1));
    }
    g[sz - 1] = std::unique_ptr<ListOfIncidents>
        (new EmptyListOfIncidents());
    
    Graph gr(g);
    StrongConnectivity builder;
    std::vector<int> comp = builder.getStronglyConnectedComponents(gr);
    Graph cond = std::move(builder.buildCondensation(gr, comp));

    ASSERT_EQ(cond.getSize(), sz);
    for (int i = 0; i < sz; i++) {
        const ListOfIncidents& li = cond.getIncidents(comp[i]);
        std::unique_ptr<Iterator> it = std::move(li.getIterator());
        if (i != sz - 1)
           ASSERT_EQ(it->getVertex(), comp[i + 1]);
        else
            ASSERT_EQ(it->getVertex(), -1);
        it->next();
        ASSERT_EQ(it->getVertex(), -1);
    }
}

TEST(CondensationTesting, CycleTest) {
    int sz = 10;
    std::vector<std::unique_ptr<ListOfIncidents> > g(sz);
    for (int i = 0; i < sz; i++) {
        g[i] = std::unique_ptr<ListOfIncidents>
            (new FunctionalListOfIncidents((i + 1) % sz));
    }

    Graph gr(g);
    StrongConnectivity builder;
    std::vector<int> comp = builder.getStronglyConnectedComponents(gr);
    Graph cond = std::move(builder.buildCondensation(gr, comp));

    ASSERT_EQ(cond.getSize(), 1);
}

/*
 * 0 -> 1 -> 2 -> 0 
 * 3 -> 4 -> 3
 * 5 -> 6 -> 5
 * 3 -> 1 
 * 4 -> 6
 */
TEST(CondensationTesting, ManualTest) {
    int sz = 7;
    std::vector<std::unique_ptr<ListOfIncidents> > g(sz);
    std::vector<LIFactory> fact(sz);
    for (int i = 0; i < sz; i++)
        fact[i] = LIFactory(sz);
    
    fact[0].addEdge(1);
    fact[1].addEdge(2);
    fact[2].addEdge(0);

    fact[3].addEdge(4);
    fact[4].addEdge(3);

    fact[5].addEdge(6);
    fact[6].addEdge(5);

    fact[3].addEdge(1);
    fact[4].addEdge(6);

    for (int i = 0; i < sz; i++)
        g[i] = std::move(fact[i].build());

    Graph gr(g);
    StrongConnectivity builder;
    std::vector<int> comp = builder.getStronglyConnectedComponents(gr);
    Graph cond = std::move(builder.buildCondensation(gr, comp));
    
    ASSERT_EQ(cond.getSize(), 3);
    
    const ListOfIncidents& li0 = cond.getIncidents(comp[0]);
    auto it = li0.getIterator();
    ASSERT_EQ(it->getVertex(), -1);
    
    const ListOfIncidents& li3 = cond.getIncidents(comp[3]);
    it = li3.getIterator();
    int v1 = it->getVertex();
    it->next();
    int v2 = it->getVertex();
    it->next();
    ASSERT_EQ(it->getVertex(), -1);
    ASSERT_TRUE((v1 == comp[0] && v2 == comp[5]) || 
                (v1 == comp[5] && v2 == comp[0]));

    const ListOfIncidents& li5 = cond.getIncidents(comp[5]);
    it = li5.getIterator();
    ASSERT_EQ(it->getVertex(), -1);
}

TEST(AddingEdgesTesting, BambooTest) {
    int sz = 10;
    std::vector<std::unique_ptr<ListOfIncidents> > g(sz);
    for (int i = 0; i < sz - 1; i++) {
        g[i] = std::unique_ptr<ListOfIncidents>
            (new FunctionalListOfIncidents(i + 1));
    }
    g[sz - 1] = std::unique_ptr<ListOfIncidents>
        (new EmptyListOfIncidents());
    
    Graph gr(g);
    StrongConnectivity builder;
    std::vector<Edge> res = builder.addEdges(gr);
    ASSERT_EQ(res.size(), 1);
    ASSERT_EQ(res[0].from, sz - 1);
    ASSERT_EQ(res[0].to, 0);
}

TEST(AddingEdgesTesting, CycleTest) {
    int sz = 10;
    std::vector<std::unique_ptr<ListOfIncidents> > g(sz);
    for (int i = 0; i < sz; i++) {
        g[i] = std::unique_ptr<ListOfIncidents>
            (new FunctionalListOfIncidents((i + 1) % sz));
    }

    Graph gr(g);
    StrongConnectivity builder;
    std::vector<Edge> res = builder.addEdges(gr);
    ASSERT_EQ(res.size(), 0);
}

/*
 * 0 -> 1 -> 2 -> 0 
 * 3 -> 4 -> 3
 * 5 -> 6 -> 5
 * 3 -> 1 
 * 4 -> 6
 */
TEST(AddingEdgesTesting, ManualTest) {
    int sz = 7;
    std::vector<std::unique_ptr<ListOfIncidents> > g(sz);
    std::vector<LIFactory> fact(sz);
    for (int i = 0; i < sz; i++)
        fact[i] = LIFactory(sz);
    
    fact[0].addEdge(1);
    fact[1].addEdge(2);
    fact[2].addEdge(0);

    fact[3].addEdge(4);
    fact[4].addEdge(3);

    fact[5].addEdge(6);
    fact[6].addEdge(5);

    fact[3].addEdge(1);
    fact[4].addEdge(6);

    for (int i = 0; i < sz; i++)
        g[i] = std::move(fact[i].build());

    Graph gr(g);
    StrongConnectivity builder;
    std::vector<Edge> res = builder.addEdges(gr);

    ASSERT_EQ(res.size(), 2);
    for (int i = 0; i < (int)res.size(); i++)
        fact[res[i].from].addEdge(res[i].to);

    std::vector<std::unique_ptr<ListOfIncidents> > new_g(sz);
    for (int i = 0; i < sz; i++) {
        new_g[i] = std::move(fact[i].build());
    }
    Graph new_gr(new_g);

    ASSERT_TRUE(isStronglyConnected(new_gr));
}

TEST(AddingEdgesTesting, StressTest) {
    int cnt_tests = 200;
    for (int test = 0; test < cnt_tests; test++) {
        int sz = 200;
        Graph gr;
        buildRandomGraph(gr, sz);
        StrongConnectivity builder;
        std::vector<Edge> res = builder.addEdges(gr);
        
        std::vector<LIFactory> fact(sz);
        for (int i = 0; i < sz; i++)
            fact[i] = LIFactory(sz);
        for (int i = 0; i < (int)res.size(); i++)
            fact[res[i].from].addEdge(res[i].to);
        for (int i = 0; i < sz; i++) {
            const ListOfIncidents& li = gr.getIncidents(i);
            auto it = li.getIterator();
            for (; it->getVertex() != -1; it->next()) {
                int to = it->getVertex();
                fact[i].addEdge(to);
            }
        }
        std::vector<std::unique_ptr<ListOfIncidents> > new_g(sz);
        for (int i = 0; i < sz; i++)
            new_g[i] = std::move(fact[i].build());
        Graph new_gr(new_g);
        ASSERT_TRUE(isStronglyConnected(new_gr));
    }
}
#endif
