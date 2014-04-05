#ifndef GRAPH_TESTING_H

#define GRAPH_TESTING_H

#include <memory>

#include "../graph.h"
#include "../factory.h"

TEST(FactoryTesting, EmptyListOfIncidentsTesting) {
    LIFactory fact(10);
    std::unique_ptr<ListOfIncidents> li = 
        std::move(fact.build());
    ASSERT_EQ("EmptyListOfIncidents", li->getType());
}

TEST(FactoryTesting, FunctionalListOfIncidentsTesting) {
    LIFactory fact(10);
    fact.addEdge(3);
    std::unique_ptr<ListOfIncidents> li = 
        std::move(fact.build());
    ASSERT_EQ("FunctionalListOfIncidents", li->getType());
}

TEST(FactoryTesting, IncidentVertexesTesting) {
    LIFactory fact(100);
    for (int i = 0; i < 3; i++)
        fact.addEdge(i);
    std::unique_ptr<ListOfIncidents> li = 
        std::move(fact.build());
    ASSERT_EQ("IncidentVertexes", li->getType());
}

TEST(FactoryTesting, MatrixOfIncidents) {
    LIFactory fact(40);
    for (int i = 0; i < 40; i++)
        fact.addEdge(i);
    std::unique_ptr<ListOfIncidents> li = 
        std::move(fact.build());
    ASSERT_EQ("MatrixOfIncidents", li->getType());
}

TEST(FactoryTesting, IntegrationTesting) {
    int cnt_tests = 1000;
    for (int i = 0; i < cnt_tests; i++) {
        int sz = 1000;
        LIFactory fact(sz);
        std::vector<int> to;
        for (int j = 0; j < sz; j++)
            if (rand() % 2) {
                to.push_back(j);
                fact.addEdge(j);
            }

        std::unique_ptr<ListOfIncidents> li = 
            std::move(fact.build());
        if (to.size() == 0)
            ASSERT_EQ("EmptyListOfIncidents", li->getType());
        else if (to.size() == 1)
            ASSERT_EQ("FunctionalListOfIncidents", li->getType());
        else {
            std::sort(to.begin(), to.end());
            bool double_edge = false;
            for (int j = 0; j < (int)to.size() - 1; j++)
                if (to[j] == to[j + 1]) {
                    double_edge = true;
                    break;
                }
            if (double_edge || sz >= 32 * (int)to.size())
                ASSERT_EQ("IncidentVertexes", li->getType());
            else
                ASSERT_EQ("MatrixOfIncidents", li->getType());
        }
    }
}

TEST(GraphTesting, IntegrationTest) {
    int cnt_tests = 100;
    for (int i = 0; i < cnt_tests; i++) {
        int sz = rand() % 100;
        std::vector<std::unique_ptr<ListOfIncidents> > g(sz);
        std::vector<std::vector<bool> > matr(sz);
        for (int j = 0; j < sz; j++)
            matr[j].resize(sz);
        for (int j = 0; j < sz; j++) {
            for (int k = 0; k < sz; k++)
                matr[j][k] = rand() % 2;
        }

        for (int j = 0; j < sz; j++) {
            LIFactory fact(sz);
            for (int k = 0; k < sz; k++)
                if (matr[j][k])
                    fact.addEdge(k);
            g[j] = std::move(fact.build());
        }

        Graph gr(g);
        for (int j = 0; j < sz; j++) {
            const ListOfIncidents& li = gr.getIncidents(j);
            for (int k = 0; k < sz; k++)
                if (matr[j][k])
                    ASSERT_TRUE(li.isConnected(k));
                else
                    ASSERT_FALSE(li.isConnected(k));
            
            std::unique_ptr<Iterator> it = std::move(li.getIterator());
            while (it->getVertex() != -1) {
                ASSERT_TRUE(matr[j][it->getVertex()]);
                matr[j][it->getVertex()] = false;
                it->next();
            }

            for (int k = 0; k < sz; k++)
                ASSERT_FALSE(matr[j][k]);
        }
    }
}
#endif
