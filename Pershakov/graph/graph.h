#ifndef GRAPH_H

#define GRAPH_H

#include <vector>

#include "list_of_incidents.h"

class Graph {
    public:
       explicit Graph(std::vector<std::unique_ptr<ListOfIncidents> > &_g) {
           g = move(_g);
       }

       const ListOfIncidents& getIncidents(int v) {
           return *g[v];
       }


    private:
        std::vector<std::unique_ptr<ListOfIncidents> > g;
};

#endif
