#ifndef GRAPH_HELPERS
#define GRAPH_HELPERS

using graph::BaseIncidence;
using graph::BaseIterator;
using graph::AdjacencyMatrixIncidence;
using graph::AdjacencyMatrixIterator;
using graph::AdjacencyListIterator;
using graph::GraphIterator;
using graph::Graph;
using graph::Coloring;


using std::vector;
using std::unique_ptr;
using std::pair;
using std::tuple;
using std::cout;
using std::endl;

// Access classes //
// ============== //
namespace graph {
class AccessAdjacencyMatrixIterator {
 public:
   AccessAdjacencyMatrixIterator(AdjacencyMatrixIterator* it_ptr)
       : it_ptr_(it_ptr) {}
   vector<bool>::const_iterator getPos() { return it_ptr_->pos_; }
   vector<bool>::const_iterator getEnd() { return it_ptr_->end_; }
   AdjacencyMatrixIterator* it_ptr_;
};
class AccessAdjacencyMatrixIncidence {
 public:
  AccessAdjacencyMatrixIncidence(AdjacencyMatrixIncidence* li_ptr)
      : li_ptr_(li_ptr) {}
  vector<bool>& getVectorBool() { return li_ptr_->adjdata_; }
  AdjacencyMatrixIncidence* li_ptr_;
};
class AccessAdjacencyListIterator {
 public:
  AccessAdjacencyListIterator(AdjacencyListIterator* it_ptr)
       : it_ptr_(it_ptr) {}
   vector<int>::const_iterator getPos() { return it_ptr_->pos_; }
   vector<int>::const_iterator getEnd() { return it_ptr_->end_; }
   AdjacencyListIterator* it_ptr_;
};
class AccessGraphIterator {
 public:
  AccessGraphIterator(GraphIterator* it_ptr) : it_ptr_(it_ptr) {}
  vector<unique_ptr<BaseIncidence>>::const_iterator getPos() {
    return it_ptr_->pos_;
  }
  vector<unique_ptr<BaseIncidence>>::const_iterator getEnd() {
    return it_ptr_->end_;
  }
  GraphIterator* it_ptr_;
};
}

// Algorithms on graph //
// =================== //
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

//  Generators  //
//  ==========  //
Graph buildSimpleAdjacencyMatrix(const vector<vector<bool>>& matrix) {
  vector<unique_ptr<BaseIncidence>> vval;
  for (auto it = matrix.begin(); it != matrix.end(); ++it)
    vval.emplace_back(new AdjacencyMatrixIncidence(*it));
  Graph graph(std::move(vval));
  return std::move(graph);
}
/** genMatrixByBoolMask(tuple<int,int> mask) Generates matrix by given mask
  *
  * get<0>(mask) -> size of graph
  * get<1>(mask) -> bitmask, that describes graph
  *
  * for example, mask = 749, size = 4
  *
  * 749 = 1011101101
  *      #9876543210
  * we enumerate bits from the lowest
  *
  *   bits correspond to the   |   so adjacency matrix for (4; 749)
  * adjacency matrix like this |        will look like this
  *    12  0  1  2             |              0 1 0 1
  *     6 13  3  4             |              1 0 1 0
  *     7  8 14  5             |              1 0 0 1
  *     9 10 11 15             |              1 0 0 0
  *
  * So, masks from [0; 2 ^ (size * (size - 1) / 2) ) = topsorted masks
  * all upper triangular adjacency matrixes with zeros on the main diagonal
  *
  * masks from [0; 2 ^ (size * (size - 1)) ) = masks without self-loops
  * all adjacency matrixes with zeros on the main diagonal
*/ vector<vector<bool>> genMatrixByBoolMask(tuple<int,int> mask) {
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

//  Output  //
//  ======  //
/* Here we declare operator << for tuple<int,int>
  * because we want to print tuple<int,int> using googletest
  * and it requires operator << for printed object
  * to be declared in the same namespace where object was declared
*/ namespace std {
  std::ostream& operator << (std::ostream& os, const tuple<int,int>& val) {
    os << std::get<0>(val) << " " << std::get<1>(val);
    return os;
  }
}//namespace std
#endif