#include <vector>

#include "graph.h"
#include "coloring.h"

using namespace std;
using namespace graph::algo;

namespace graph {
namespace testtools {
bool findFinishDFS(const Graph& graph,
                   int vertex_id, int finish_id, 
                   vector<bool>& is_visited)
{  
  is_visited[vertex_id] = true;
  if (vertex_id == finish_id)
    return 1;

  bool is_found = false;
  for (auto it = graph.begin(vertex_id); it->isValid(); it->moveForvard()) {
    if (!is_visited[it->get()]) {
      is_found = is_found || findFinishDFS(graph,
                                           it->get(),
                                           finish_id,
                                           is_visited);
    }
  }
  return is_found;
}
bool isPath(const Graph& graph, int start_id, int finish_id) {
  vector<bool> is_visited(graph.size(), 0);
  return findFinishDFS(graph, start_id, finish_id, is_visited);
}
Coloring getStronglyConnectedComponentsDummy(const Graph& graph) {
  Coloring strong_components;
  strong_components.color.resize(graph.size(), -1);
  int currentColor = 0;
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard()) {
    if (strong_components.color[it->get()] == -1)
    {
      strong_components.color[it->get()] = currentColor;
      strong_components.delegate.push_back(it->get());
      for (auto jt = graph.begin(-1); jt->isValid(); jt->moveForvard()) {
        if ((isPath(graph, jt->get(), it->get())) &&
            (isPath(graph, it->get(), jt->get()))) {
          strong_components.color[jt->get()] = currentColor;
        }
      }
      ++currentColor;
    }
  }
  return strong_components;
}
bool hasSelfLoop(const Graph& graph) {
  bool ans = false;
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard())
    ans = ans || graph.isConnected(it->get(), it->get());
  return ans;
}
bool hasLoopDummy(const Graph& graph) {
  if (hasSelfLoop(graph))
    return 1;
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard()) {
    for (auto jt = graph.begin(-1); jt->isValid(); jt->moveForvard()) {
      if ((it->get() != jt->get()) &&
          isPath(graph, it->get(), jt->get()) &&
          isPath(graph, jt->get(), it->get()))
        return 1;
    }
  }
  return 0;
}
}//namespace testtools
}//namespace graph