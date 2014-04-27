#ifndef TESTING_UTILITIES_H

#define TESTING_UTILITIES_H

#include <vector>
#include <memory>

#include "../list_of_incidents.h"
#include "../graph.h"
#include "../factory.h"

void buildRandomGraph(Graph &gr, int sz) {
    std::vector<std::unique_ptr<ListOfIncidents> > g(sz);

    for (int i = 0; i < sz; i++) {
        LIFactory fact(sz);
        for (int j = 0; j < sz; j++)
            if (rand() % 30 == 0 && i != j)
                fact.addEdge(j);
        g[i] = std::move(fact.build());
    }
    gr = std::move(Graph(g));
}

void dfsCheck(const Graph &gr, int v, std::vector<bool> &used) {
    used[v] = true;
    const ListOfIncidents& li = gr.getIncidents(v);
    auto it = std::move(li.getIterator());
    for (; it->getVertex() != -1; it->next()) {
        int to = it->getVertex();
        if (!used[to])
            dfsCheck(gr, to, used);
    }
}

bool isStronglyConnected(const Graph &gr) {
    int sz = gr.getSize();
    for (int i = 0; i < sz; i++) {
        std::vector<bool> used(sz, false);
        dfsCheck(gr, i, used);
        for (int j = 0; j < sz; j++)
            if (!used[j])
                return false;
    }
    return true;
}

#endif
