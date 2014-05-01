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

bool hasSelfLoop(const Graph& graph) {
  bool ans = 0;
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard())
    ans = ans || graph.isConnected(it->get(), it->get());
  return ans;
}


namespace {

int tarjanFindSinkDFS(int v, const Graph& graph,
                      vector<bool>& is_visited, vector<bool>& is_sink) {
  is_visited[v] = 1;
  if (is_sink[v])
    return v;
  int found_sink = -1;
  for (auto it = graph.begin(v); it->isValid(); it->moveForvard()) {
    if (!is_visited[it->get()]) {
      found_sink = tarjanFindSinkDFS(it->get(), graph, is_visited, is_sink);
      if (found_sink != -1) {
        return found_sink;
      }
    }
  }
  return found_sink;
}

}//anonymous namespace

vector<pair<int,int>> getCompletionToStrongСonnectivityInСondensed(
                                                         const Graph& graph) {
  //declare variable to hold answer
  vector<pair<int, int>> completion;

  //check whether the call is correct and initialize isolated, source, sink 
  if (hasSelfLoop(graph)) {
    cerr << "In getCompletionToStrongСonnectivityInСondensed" << endl
         << "graph has self-loop" << endl;
         abort();
  }
  vector<int> isolated = getIsolated(graph);
  vector<int> source = getSource(graph);
  vector<int> sink = getSink(graph);
  if(source.size() > sink.size()) {
    cerr << "In getCompletionToStrongСonnectivityInСondensed" << endl
         << "source.size() = " << source.size() << " > "
         << "sink.size() = " << sink.size() << endl;
    abort();
  }

  //trivial case = just isolated vertexes
  if (source.empty() && sink.empty()) {  
    if (isolated.size() <= 1)
      return vector<pair<int, int>>();
    else {
      for(size_t i = 0; i < isolated.size(); ++i)
        completion.emplace_back(i, (i + 1) % isolated.size());
      return completion;
    }
  }

  //declaration of some useful values
  vector<bool> is_sink(graph.size(), 0);
  for (auto it = sink.begin(); it != sink.end(); ++it) {
    is_sink[*it] = 1;
  }  
  vector<bool> is_visited(graph.size(), 0);
  vector<int> unused_source;
  vector<int> unused_sink;
  int last_found_sink = -1;
  int first_source = -1;
  
  //launches of the main dfs in algorithm
  for (auto it = source.begin(); it != source.end(); ++it) {
    int current_found_sink = tarjanFindSinkDFS(*it, graph, is_visited, is_sink);
    if (current_found_sink != -1) {
      if (last_found_sink == -1) {
        first_source = *it;
      } else {
        completion.emplace_back(last_found_sink, *it);
      }
      last_found_sink = current_found_sink;
    } else {
      unused_source.push_back(*it);
    }
  }

  //fill the array of unused sinks (not reached in dfs)
  for (auto it = sink.begin(); it != sink.end(); ++it) {
    if (!is_visited[*it])
      unused_sink.push_back(*it);
  }

  //adding edges between sources and sinks
  for (size_t i = 0; i < unused_source.size(); ++i) {
    completion.emplace_back(unused_sink[i], unused_source[i]);
  }
  for (size_t i = unused_source.size(); i < unused_sink.size(); ++i) {
    completion.emplace_back(unused_sink[i], first_source);
  }
  completion.emplace_back(last_found_sink, first_source);

  //adding isolated vertexes
  if (!isolated.empty()) {
    pair<int,int> last_added_edge = completion.back();
    completion.pop_back();
    completion.emplace_back(last_added_edge.first, isolated.front());
    completion.emplace_back(isolated.back(), last_added_edge.second);
    for (size_t i = 0; i < (isolated.size() - 1); ++i) {
      completion.emplace_back(isolated[i], isolated[i + 1]);
    }
  }

  //tadam! return result
  return completion;
}
}//namespace graph