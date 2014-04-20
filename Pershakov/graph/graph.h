#ifndef GRAPH_H

#define GRAPH_H

#include <vector>
#include <memory>

#include "list_of_incidents.h"

class Graph {
    public:
        Graph() {
            sz = 0;
        }

        explicit Graph(std::vector<std::unique_ptr<ListOfIncidents> > &_g) {
            g = std::move(_g);
            sz = g.size();
        }

        Graph(Graph &&gr) : g(std::move(gr.g)), sz(gr.sz) { 
            gr.sz = 0;
        }

        Graph& operator= (Graph &&gr) { 
            sz = gr.sz;
            g = std::move(gr.g);
            gr.sz = 0;
            return *this;
        }

        const ListOfIncidents& getIncidents(int v) const {
            return *g[v];
        }
        
        int getSize() const {
            return sz;
        }

    private:
        std::vector<std::unique_ptr<ListOfIncidents> > g;
        int sz;
};

#endif
