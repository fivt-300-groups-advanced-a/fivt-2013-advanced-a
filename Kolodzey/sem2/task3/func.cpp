#include "graph.h"
#include <vector>

using graph::Graph;
using std::vector;


namespace graph {

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
struct EnviromentForInnerHasLoop {
  EnviromentForInnerHasLoop() : graph_(nullptr), visited_(nullptr) {}
  const Graph* graph_;
  vector<int>* visited_;
};
bool innerHasLoop(int v, const EnviromentForInnerHasLoop& env) {
  env.visited_->at(v) = 1;
  bool ans = false;
  for (auto it = env.graph_->begin(v); it->isValid(); it->moveForvard()) {
    if (env.visited_->at(it->get()) == 1)
      return 1;
    else if (env.visited_->at(it->get()) == 0)
      ans = ans || innerHasLoop(it->get(), env);
  }
  env.visited_->operator[](v) = 2;
  return ans;
}
}//anonymous namespace
bool hasLoop(const Graph& graph) {
  if (graph.size() == 0)
    return 0;
  bool ans = false;
  vector<int> visited(graph.size(), 0);
  EnviromentForInnerHasLoop env;
  env.graph_ = &graph;
  env.visited_ = &visited;
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard()) {
    if (visited[it->get()] == 0) {
      ans = ans || innerHasLoop(it->get(), env);
    }
  }
  return ans;
}

namespace {
struct EnviromentForTarjanFindSinkDFS {
  EnviromentForTarjanFindSinkDFS()
    : graph_(nullptr), is_visited_(nullptr), is_sink_(nullptr) {}
  EnviromentForTarjanFindSinkDFS (const Graph* graph,
                                  vector<bool>* is_visited,
                                  vector<bool>* is_sink)
    : graph_(graph), is_visited_(is_visited), is_sink_(is_sink) {}
  const Graph* graph_;
  vector<bool>* is_visited_;
  vector<bool>* is_sink_;
};
int tarjanFindSinkDFS(int v, const EnviromentForTarjanFindSinkDFS& env) {
  env.is_visited_->at(v) = 1;
  if (env.is_sink_->at(v))
    return v;
  int found_sink = -1;
  for (auto it = env.graph_->begin(v); it->isValid(); it->moveForvard()) {
    if (!(env.is_visited_->at(it->get()))) {
      found_sink = tarjanFindSinkDFS(it->get(), env);
      if (found_sink != -1) {
        return found_sink;
      }
    }
  }
  return found_sink;
}
}//anonymous namespace
vector<pair<int,int>> getCompletionToStrongСonnectivityInСondensed(
                                                         const Graph& graph)
{
  //declare variable to hold answer
  vector<pair<int, int>> completion;

  //check whether the call is correct and initialize isolated, source, sink 
  if (hasLoop(graph)) {
    cerr << "In getCompletionToStrongСonnectivityInСondensed" << endl
         << "graph has loop" << endl;
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
  
  EnviromentForTarjanFindSinkDFS env;
  env.graph_ = &graph;
  env.is_visited_ = &is_visited;
  env.is_sink_ = &is_sink;
  //launches of the main dfs in algorithm
  for (auto it = source.begin(); it != source.end(); ++it) {
    int current_found_sink = tarjanFindSinkDFS(*it, env);
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

namespace {
struct EnviromentForDfsStronglyConnected {
  const Graph* graph_;
  int* entertime_;
  vector<bool>* visited_;
  vector<int>* lowlink_;
  vector<int>* stack_;
  Coloring* components_;
};
void dfsStronglyConnected(int v, const EnviromentForDfsStronglyConnected& env) {
  int INF = (env.graph_)->size() + 100500;

  (env.visited_)->at(v) = true;
  (env.lowlink_)->at(v) = ++(*(env.entertime_));
  (env.stack_)->push_back(v);

  for (auto it = env.graph_->begin(v); it->isValid(); it->moveForvard()) {
    if (!(env.visited_->at(it->get())))
      dfsStronglyConnected(it->get(), env);
  }

  bool isRoot = true;
  for (auto it = env.graph_->begin(v); it->isValid(); it->moveForvard())
    if (env.lowlink_->at(it->get()) < env.lowlink_->at(v))
    {  
      isRoot = false;
      env.lowlink_->at(v) = env.lowlink_->at(it->get());
    }
  if (isRoot) {
    int color = env.components_->getNumberOfColors();
    env.components_->color[v] = color;
    env.components_->delegate.push_back(v);
    int u = v;
    do {
      u = env.stack_->back();
      env.stack_->pop_back();
      env.components_->color[u] = color;
      env.lowlink_->at(u) = INF;
    } while (u != v);
  }
}
}//anonymous namespace

Coloring getStronglyConnectedComponents(const Graph& graph) {
  int entertime = 0;
  vector<bool> visited(graph.size(), 0);
  vector<int> lowlink(graph.size());
  vector<int> stack;
  Coloring components;
  components.color.resize(graph.size());
  EnviromentForDfsStronglyConnected env;
  env.graph_ = &graph;
  env.entertime_ = &entertime;
  env.visited_ = &visited;
  env.lowlink_ = &lowlink;
  env.stack_ = &stack;
  env.components_ = &components;
  for (auto it = graph.begin(-1); it->isValid(); it->moveForvard()) {
    if (!visited[it->get()])
      dfsStronglyConnected(it->get(), env);
  }
  return components;
}
}//namespace graph