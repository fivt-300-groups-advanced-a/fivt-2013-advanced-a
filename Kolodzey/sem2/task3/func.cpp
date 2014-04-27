#include "graph.h"
#include <vector>

using graph::Graph;
using std::vector;

namespace {
bool findFinishDFS(const Graph& graph,
                   int vertex_id, int finish_id, 
                   vector<bool>& is_visited) {
  
  is_visited[vertex_id] = true;

  if (vertex_id == finish_id)
    return 1;

  bool is_found = false;
  for (auto it = graph.begin(vertex_id); it->isValid(); it->moveForvard()) {
    if (!is_visited[it->getCurrentVertexId()]) {
      is_found = is_found || findFinishDFS(graph,
                                           it->getCurrentVertexId(),
                                           finish_id,
                                           is_visited);
    }
  }
  return is_found;
} 

}//anonymous namespace

namespace graph {

bool isPath(const Graph& graph, int start_id, int finish_id) {
  vector<bool> is_visited(graph.size(), 0);
  return findFinishDFS(graph, start_id, finish_id, is_visited);
}

}//namespace graph