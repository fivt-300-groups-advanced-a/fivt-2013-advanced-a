#ifndef DUMMYALGO_H
#define DUMMYALGO_H

#include <vector>

#include "graph.h"
#include "coloring.h"

namespace graph {
namespace testtools {
bool findFinishDFS(const Graph& graph,
                   int vertex_id, int finish_id, 
                   std::vector<bool>& is_visited);
bool isPath(const Graph& graph, int start_id, int finish_id);
algo::Coloring getStronglyConnectedComponentsDummy(const Graph& graph);
bool hasSelfLoop(const Graph& graph);
bool hasLoopDummy(const Graph& graph);
}//namespace testtools
}//namespace graph
#endif //DUMMYALGO_H