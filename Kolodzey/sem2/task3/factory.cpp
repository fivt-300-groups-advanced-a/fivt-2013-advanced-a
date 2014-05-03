#include "graph.h"

namespace graph {
Graph buildSimpleAdjacencyMatrix(const vector<vector<bool>>& matrix) {
  vector<unique_ptr<BaseIncidence>> vval;
  for (auto it = matrix.begin(); it != matrix.end(); ++it)
    vval.emplace_back(new AdjacencyMatrixIncidence(*it));
  Graph graph(std::move(vval));
  return std::move(graph);
}

}//namespace graph