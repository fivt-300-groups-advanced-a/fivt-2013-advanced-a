#ifndef STRONG_CONNECTIVITY_AUGMENTATION
#define STRONG_CONNECTIVITY_AUGMENTATION

#include "graph.h"
#include <stack>
#include <vector>

class StrongConnectivityAugmentation {
  public:
    StrongConnectivityAugmentation(const Graph& _graph):graph(_graph) {
      colors.resize(graph.size());
      build_strongly_connected_components();
      build_strong_connectivity_augmentation();
    }

    std::vector<std::pair<unsigned int, unsigned int> > get_answer() {
      return result;
    }

    void build_strong_connectivity_augmentation() {
      unsigned int max_color = 0;
      for (unsigned int v=0;v<graph.size();v++) max_color = std::max(max_color, colors[v]);
      if (max_color==0) return;

      std::vector<char> has_input(max_color+1);
      std::vector<char> has_output(max_color+1);
      for (int v=0;v<graph.size();v++) {
        auto it = graph.get_list(v).get_iterator();
        while (it->valid()) {
          if (colors[v]!=colors[it->current()]) {
            has_output[colors[v]] = 1;
            has_input[colors[it->current()]] = 1;
          }
          it->next();
        }
      }

      std::vector<std::pair<unsigned int, unsigned int>> pairs;
      std::vector<char> used(graph.size());
      for (unsigned int v=0;v<graph.size();v++) {
        if (!has_input[colors[v]]) {
          int fit = find_fit_vertex(v, has_output, used);
          if (fit!=-1) {
            pairs.push_back(std::make_pair(v, fit));
            has_input[colors[v]] = 1;
            has_output[colors[fit]] = 1;
          }
        }
      }

      for (unsigned int i=0;i<pairs.size();i++)
        result.push_back(std::make_pair(pairs[i].second, pairs[(i+1)%pairs.size()].first));

      std::vector<unsigned int> vertexes_need_out, vertexes_need_in;
      for (unsigned int v=0;v<graph.size();v++) {
        if (!has_input[colors[v]])
          vertexes_need_in.push_back(v);
        if (!has_output[colors[v]])
          vertexes_need_out.push_back(v);
      }
      while (!vertexes_need_out.empty() && !vertexes_need_in.empty()) {
        result.push_back(std::make_pair(vertexes_need_out.back(), vertexes_need_in.back()));
        vertexes_need_out.pop_back();
        vertexes_need_in.pop_back();
      }
      while (!vertexes_need_in.empty()) {
        result.push_back(std::make_pair(result.back().first, vertexes_need_in.back()));
        vertexes_need_in.pop_back();
      }
      while (!vertexes_need_out.empty())
      {
        result.push_back(std::make_pair(vertexes_need_out.back(), result.back().second));
        vertexes_need_out.pop_back();
      }
    }

  private:
    void tarjan_dfs(unsigned int v, unsigned int &current_color, std::stack<unsigned int>& used_stack, std::vector<char>& used, unsigned int& current_time, std::vector<unsigned int>& min_time, std::vector<unsigned int>& tin) {
      used_stack.push(v);
      tin[v] = current_time;
      min_time[v] = current_time++;
      used[v] = 1;

      auto it = graph.get_list(v).get_iterator();
      while (it->valid()) {
        if (!used[it->current()])
          tarjan_dfs(it->current(), current_color, used_stack, used, current_time, min_time, tin);
        it->next();
      }
      it = graph.get_list(v).get_iterator();

      while (it->valid()) {
        min_time[v] = std::min(min_time[v], min_time[it->current()]);
        it->next();
      }
      if (min_time[v] == tin[v]) {
        unsigned int last_vertex;
        do {
          last_vertex = used_stack.top();
          used_stack.pop();
          colors[last_vertex] = current_color;
        } while (last_vertex != v);
        current_color++;
      }
    }

    void build_strongly_connected_components() {
      unsigned int current_color = 0, current_time = 0;
      std::stack<unsigned int> used_stack;
      std::vector<char> used(graph.size());
      std::vector<unsigned int> tin(graph.size());
      std::vector<unsigned int> min_time(graph.size());
      for (unsigned int v=0;v<graph.size();v++)
        if (!used[v]) 
          tarjan_dfs(v, current_color, used_stack, used, current_time, min_time, tin);
    }

    unsigned int find_fit_vertex(unsigned int vertex, std::vector<char>& has_output, std::vector<char>& used) {
      used[vertex] = 1;
      if (!has_output[colors[vertex]])
        return vertex;
      auto it = graph.get_list(vertex).get_iterator();
      int result = -1;
      while (it->valid())
      {
        unsigned int current_vertex = it->current();
        if (!used[current_vertex])
        {
           if ((result = find_fit_vertex(current_vertex, has_output, used)) != -1)
             return result;
        }
        it->next();
      }
      return result;
    }
    friend class TesterSCA;

    const Graph& graph;
    std::vector<unsigned int> colors;
    std::vector<std::pair<unsigned int, unsigned int> > result;
};
#endif
