#include "graph.h"
#include <algorithm>
#include <cassert>

std::vector<std::unique_ptr<ListOfIncidents> > map(int n, std::function<ListOfIncidents*(int)> pred) {
    std::vector<std::unique_ptr<ListOfIncidents> > vv;
    for (int i = 0; i < n; i++)
        vv.push_back(std::unique_ptr<ListOfIncidents>(pred(i)));
    return vv;
}

