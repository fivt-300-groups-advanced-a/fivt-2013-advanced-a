#include "graph.h"
#include "implementations.h"

using namespace std;

namespace graph {
namespace testtools {
Graph buildSimpleAdjacencyMatrix(const vector<vector<bool>>& matrix) {
  vector<unique_ptr<BaseIncidence>> vval;
  for (auto it = matrix.begin(); it != matrix.end(); ++it)
    vval.emplace_back(new AdjacencyMatrixIncidence(*it));
  Graph graph(std::move(vval));
  return std::move(graph);
}
vector<vector<bool>> genMatrixByBoolMask(tuple<int,int> mask) {
  int size = std::get<0>(mask);
  int bitmask = std::get<1>(mask);
  vector<vector<bool>> matrix(size, vector<bool>(size, 0));
  int currentbit = 0;
  for(int i = 0; i < size; ++i)
    for (int j = i + 1; j < size; ++j) {
      matrix[i][j] = (bitmask & (1 << currentbit)) > 0;
      ++currentbit;
    }
  for(int i = 0; i < size; ++i)
    for (int j = 0; j < i; ++j) {
      matrix[i][j] = (bitmask & (1 << currentbit)) > 0;
      ++currentbit;
    }
  for (int i = 0; i < size; ++i) {
     matrix[i][i] = (bitmask & (1 << currentbit)) > 0;
     ++currentbit;
  }
  return matrix;
}
}//namespace testtools
}//namespace graph