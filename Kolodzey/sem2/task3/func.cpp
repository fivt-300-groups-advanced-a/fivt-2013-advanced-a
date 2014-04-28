#include "graph.h"
#include <vector>

using graph::Graph;
using std::vector;


namespace graph {

namespace {

bool findFinishDFS(const Graph& graph,
                   int vertex_id, int finish_id, 
                   vector<bool>& is_visited) {
  
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

}//anonymous namespace
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
      strong_components.representative.push_back(it->get());
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



vector<int> getSource(const Graph& graph) {
  
  vector<bool> is_incomeless(graph.size(), 1);
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard()) {
    for (auto jt = graph.begin(it->get()); jt->isValid(); jt->moveForvard()) {
      is_incomeless[jt->get()] = 0;
    }
  }

  vector<int> ans;
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard())
    if ((is_incomeless[it->get()]) && (graph.begin(it->get())->isValid()))
      ans.push_back(it->get());

  return ans;
}

vector<int> getSink(const Graph& graph) {
  
  vector<bool> has_income(graph.size(), 0);
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard()) {
    for (auto jt = graph.begin(it->get()); jt->isValid(); jt->moveForvard()) {
      has_income[jt->get()] = 1;
    }
  }

  vector<int> ans;
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard())
    if ((has_income[it->get()]) && (!graph.begin(it->get())->isValid()))
      ans.push_back(it->get());
    
  return ans;
}

vector<int> getIsolated(const Graph& graph) {
  
  vector<bool> is_incomeless(graph.size(), 1);
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard()) {
    for (auto jt = graph.begin(it->get()); jt->isValid(); jt->moveForvard()) {
      is_incomeless[jt->get()] = 0;
    }
  }

  vector<int> ans;
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard())
    if ((is_incomeless[it->get()]) && (!graph.begin(it->get())->isValid()))
      ans.push_back(it->get());

  return ans;
}

namespace {

int tarjanFindSickDFS(int v, const Graph& graph,
                      vector<bool>& is_visited, vector<bool>& is_sink) {
  is_visited[v] = 1;
  if (is_sink[v])
    return v;
  int found_sink = -1;
  for (auto it = graph.begin(v); it->isValid(); it->moveForvard()) {
    if (!is_visited[it->get()]) {
      found_sick = tarjanFindSickDFS(it->get(), graph, is_visited, is_sink);
      if (found_sick != -1) {
        return found_sick;
      }
    }
  }
  return found_sick;
}

}//anonymous namespace

vector<pair<int,int>> getCompletionToStrongСonnectivityInСondensed(
                                                         const Graph& graph) {
  vector<int> isolated = getIsolated(graph);
  vector<int> source = getSource(graph);
  vector<int> sink = getSink(graph);
  vector<bool> is_sink(graph.size(), 0);
  for (auto it = sink.begin(); it != sink.end(); ++it) {
    is_sink[*it] = 1;
  }
  vector<int> unused_source;
  vector<int> unused_sink;

  vector<pair<int, int>> completion;

  vector<bool> is_visited(graph.size(), 0);
  int last_found_sick = -1;
  int first_source = -1;
  
  for (auto it = source.begin(); it != source.end(); ++it) {
    current_found_sick = tarjanFindSickDFS(*it, graph, is_visited, is_sink);
    if (current_found_sick != -1) {
      if (last_found_sick == -1) {
        first_source = *it;
      } else {
        completion.push_back(last_found_sick, *it);
      }
      last_found_sick = current_found_sick;
    } else {
      unused_source.push_back(*it);
    }
  }

  for (auto it = sink.begin(); it != sink.end(); ++it) {
    if (!is_visited[*it])
      unused_sink.push_back(*it);
  }

  if ((isolated.size() == 1) && (first_source == -1))


}


}//namespace graph