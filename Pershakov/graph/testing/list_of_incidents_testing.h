#ifndef LIST_OF_INCIDENTS_TESTING_H

#define LIST_OF_INCIDENTS_TESTING_H

#include <vector>
#include <algorithm>

#include "../list_of_incidents.h"

TEST(ListOfIncidentsTesting, EmptyListOfIncidentsTesting) {
    EmptyListOfIncidents li;
    int cnt_tests = 1000;
    for (int i = 0; i < cnt_tests; i++) {
        int v = rand();
        ASSERT_FALSE(li.isConnected(v));
    }
}

TEST(ListOfIncidentsTesting, FunctionalListOfIncidentsTesting) {
    int cnt_tests = 1000;
    for (int i = 0; i < cnt_tests; i++) {
        int to = rand() % 1000;
        FunctionalListOfIncidents li(to);
        for (int j = 0; j < cnt_tests; j++) {
            int v = rand() % 1000;
            if (v == to)
                ASSERT_TRUE(li.isConnected(v));
            else
                ASSERT_FALSE(li.isConnected(v));
        }
        ASSERT_TRUE(li.isConnected(to));
    }
}

TEST(ListOfIncidentsTesting, MatrixOfIncidentsTesting) {
    int cnt_tests = 1000;
    for (int i = 0; i < cnt_tests; i++) {
        int sz = rand() % 1000;
        std::vector<bool> adj(sz);
        for (int j = 0; j < sz; j++)
            adj[j] = rand() % 2;
        MatrixOfIncidents li(adj);
        for (int j = 0; j < sz; j++)
            ASSERT_EQ(adj[j], li.isConnected(j));
    }
}

TEST(ListOfIncidentsTesting, IncidentVertexesTesting) {
    int cnt_tests = 1000;
    for (int i = 0; i < cnt_tests; i++) {
        int sz = rand() % 1000;
        std::vector<bool> adj(sz);
        std::vector<int> to;
        for (int j = 0; j < sz; j++) {
            adj[j] = rand() % 2;
            if (adj[j])
                to.push_back(j);
        }
        std::random_shuffle(to.begin(), to.end());
        IncidentVertexes li(to);
        for (int j = 0; j < sz; j++)
            ASSERT_EQ(adj[j], li.isConnected(j));
    }
}

#endif
