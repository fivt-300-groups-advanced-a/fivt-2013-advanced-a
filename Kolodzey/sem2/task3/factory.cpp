#include "graph.h"

namespace graph {
Graph simpleAdjacencyMatrixFabric(int size, bool matrix[]) {
  vector<unique_ptr<BaseIncidence>> vval;
  for (int i = 0; i < size; ++i)
    vval.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(matrix + (size * i),
                                           matrix + ((size * i) + size))));
  return Graph(std::move(vval));
}
}//namespace graph