#ifndef FACTORY_H

#define FACTORY_H

#include <vector>
#include <memory>

#include "list_of_incidents.h"

class LIFactory {
    public:
        LIFactory() { 
            cnt_vertexes = 1;
        }

        explicit LIFactory(int cnt) { 
            cnt_vertexes = cnt;
        }

        LIFactory(const std::vector<int> &new_to, int cnt) {
            to = new_to;
            cnt_vertexes = cnt;
        }

        void addEdge(int v) {
            to.push_back(v);
        }

        void unique() {
            std::sort(to.begin(), to.end());
            auto it = std::unique(to.begin(), to.end());
            to.resize(it - to.begin());
        }

        std::unique_ptr<ListOfIncidents> build() {
            std::unique_ptr<ListOfIncidents> res;
            if (to.size() == 0)
                return std::move(
                    std::unique_ptr<ListOfIncidents>(
                        new EmptyListOfIncidents()));

            if (to.size() == 1)
                return std::move(
                    std::unique_ptr<ListOfIncidents>(
                        new FunctionalListOfIncidents(to[0])));

            if (cnt_vertexes < 32 * (int)to.size()) {
                std::vector<bool> adj(cnt_vertexes);
                bool double_edges = false;
                for (int i = 0; i < (int)to.size(); i++) {
                    if (adj[to[i]]) {
                        double_edges = true;
                        break;
                    }
                    adj[to[i]] = true;
                }
                if (!double_edges)
                    return std::move(
                        std::unique_ptr<ListOfIncidents>(
                            new MatrixOfIncidents(adj)));
            }
            
            return std::move(
                std::unique_ptr<ListOfIncidents>(
                    new IncidentVertexes(to)));
        }

    private:
        std::vector<int> to;
        int cnt_vertexes;
};

#endif
