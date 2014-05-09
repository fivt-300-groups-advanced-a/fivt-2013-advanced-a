#ifndef ALGO_H
#define ALGO_H

#include "graph.h"
#include "coloring.h"

namespace graph {
namespace algo {
std::vector<int> getSource(const Graph& graph);
std::vector<int> getSink(const Graph& graph);
std::vector<int> getIsolated(const Graph& graph);
bool hasLoop(const Graph& graph); //even for self-loop
Graph reverseGraph(const Graph& graph);
Graph condenceGraph(const Graph& graph, const Coloring& components);
Coloring getStronglyConnectedComponents(const Graph& graph);
std::vector<std::pair<int,int>> getCompletionToStrongСonnectivityInСondensed(
                                                         const Graph& graph);
std::vector<std::pair<int,int>> getCompletionToStrongСonnectivity(
                                                         const Graph& graph);
}//namespace algo
}//namespace graph
#endif //ALGO_H