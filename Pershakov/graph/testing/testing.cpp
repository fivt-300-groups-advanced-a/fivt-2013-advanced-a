#include <gtest/gtest.h>
#include <vector>
#include <memory>

#include "../graph.h"

TEST(GraphTesting, FunctionalGraphTesting) {
    int cnt_tests = 1000;
    for (int test = 0; test < cnt_tests; test++) {
        int sz = rand() % 1000 + 1;
        std::vector<std::unique_ptr<ListOfIncidents> > gr(sz);
        std::vector<int> to(sz);
        for (int i = 0; i < sz; i++) {
            to[i] = rand() % sz;
            gr[i] = std::move(std::unique_ptr<ListOfIncidents>
                (new FunctionalListOfIncidents(to[i])));
        }
        Graph g(gr);

        for (int i = 0; i < sz; i++) {
            const ListOfIncidents& li = g.getIncidents(i);
            for (int j = 0; j < sz; j++)
                if (j == to[i])
                    ASSERT_TRUE(li.isConnected(j));
                else
                    ASSERT_FALSE(li.isConnected(j));
            
            auto it = li.getIterator();
            ASSERT_EQ(it->getVertex(), to[i]);
            it->next();
            ASSERT_EQ(it->getVertex(), -1);
        }
    }
}

TEST(GraphTesting, MatrixOfIncidentsTesting) {
    int cnt_tests = 100;
    for (int test = 0; test < cnt_tests; test++) {
        int sz = rand() % 1000 + 1;
        std::vector<std::unique_ptr<ListOfIncidents> > gr(sz);
        std::vector<std::vector<bool> > matr(sz);
        for (int i = 0; i < sz; i++) {
            matr[i].resize(sz, false);
            for (int j = 0; j < sz; j++)
                if (rand() % 2)
                    matr[i][j] = true;
            gr[i] = std::move(std::unique_ptr<ListOfIncidents>
                (new MatrixOfIncidents(matr[i])));
        }
        Graph g(gr);

        for (int i = 0; i < sz; i++) {
            const ListOfIncidents& li = g.getIncidents(i);
            for (int j = 0; j < sz; j++)
                if (matr[i][j])
                    ASSERT_TRUE(li.isConnected(j));
                else
                    ASSERT_FALSE(li.isConnected(j));
            
            auto it = li.getIterator();
            while (it->getVertex() != -1) {
                ASSERT_TRUE(matr[i][it->getVertex()]);
                matr[i][it->getVertex()] = false;
                it->next();
            }
        }

        for (int i = 0; i < sz; i++)
            for (int j = 0; j < sz; j++)
                ASSERT_FALSE(matr[i][j]);
    }
}

TEST(GraphTesting, IncidentVertexesTesting) {
    int cnt_tests = 100;
    for (int test = 0; test < cnt_tests; test++) {
        int sz = rand() % 1000 + 1;
        std::vector<std::unique_ptr<ListOfIncidents> > gr(sz);
        std::vector<std::vector<bool> > matr(sz);
        std::vector<int> adj;
        for (int i = 0; i < sz; i++) {
            matr[i].resize(sz, false);
            for (int j = 0; j < sz; j++)
                if (rand() % 2)
                    matr[i][j] = true;
            adj.clear();
            for (int j = 0; j < sz; j++)
                if (matr[i][j])
                    adj.push_back(j);
            std::random_shuffle(adj.begin(), adj.end());
            gr[i] = std::move(std::unique_ptr<ListOfIncidents>
                (new IncidentVertexes(adj)));
        }
        Graph g(gr);

        for (int i = 0; i < sz; i++) {
            const ListOfIncidents& li = g.getIncidents(i);
            for (int j = 0; j < sz; j++)
                if (matr[i][j])
                    ASSERT_TRUE(li.isConnected(j));
                else
                    ASSERT_FALSE(li.isConnected(j));
            
            auto it = li.getIterator();
            while (it->getVertex() != -1) {
                ASSERT_TRUE(matr[i][it->getVertex()]);
                matr[i][it->getVertex()] = false;
                it->next();
            }
        }

        for (int i = 0; i < sz; i++)
            for (int j = 0; j < sz; j++)
                ASSERT_FALSE(matr[i][j]);
    }
}

TEST(GraphTesting, KHeapTesting) {
    int cnt_tests = 100;
    for (int test = 0; test < cnt_tests; test++) {
        int sz = rand() % 1000 + 1;
        int k = rand() % sz;
        std::vector<std::unique_ptr<ListOfIncidents> > gr(sz);
        for (int i = 0; i < sz; i++){
            int l = i * k + 1, r = (i + 1) * k;
            l = std::min(sz, l);
            r = std::min(r, sz - 1);
            gr[i] = std::move(std::unique_ptr<ListOfIncidents>
                (new KHeapListOfIncidents(l, r)));
        }
        Graph g(gr);

        for (int i = 0; i < sz; i++) {
            const ListOfIncidents& li = g.getIncidents(i);
            int l = i * k + 1, r = (i + 1) * k;
            for (int j = 0; j < sz; j++){ 
                if (l <= j && j <= r) {
                    ASSERT_TRUE(li.isConnected(j));
                } else {
                    ASSERT_FALSE(li.isConnected(j));
                }
            }
            
            l = std::min(sz, l);
            r = std::min(r, sz - 1);
            auto it = li.getIterator();
            int cnt = l == sz? 0 : r - l + 1;
            while (it->getVertex() != -1) {
                ASSERT_TRUE(l <= it->getVertex() && it->getVertex() <= r);
                it->next();
                cnt--;
            }
            ASSERT_TRUE(cnt == 0);
        }

    }
}

int main(int argc, char **argv){
    srand(44);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
