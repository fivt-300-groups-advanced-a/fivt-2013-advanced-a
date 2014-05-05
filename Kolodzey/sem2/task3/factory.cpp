#include "graph.h"

namespace graph {
enum Implementation {
  IMP_BIT,
  IMP_LIST,
  IMP_ONE
};
class IncidenceFactory {
 public: 
  IncidenceFactory(size_t supported_size_of_graph)
                                : graph_size_(supported_size_of_graph) { }
  unique_ptr<BaseIncidence> getIncidence() {
    if (getCurrentSize() == 1)
      return unique_ptr<BaseIncidence>(new OneVertexIncidence(list_[0]));
    if (isBit())
      return unique_ptr<BaseIncidence>(new AdjacencyMatrixIncidence(bit_));
    return unique_ptr<BaseIncidence>(new AdjacencyListIncidence(list_));
  }
  void addEdge(int v) {
    if(isBit())
      bit_[v] = 1;
    else
      list_.push_back(v);
    if((!isBit()) && (getCurrentSize() > (graph_size_ / 32)) && 
                     (getCurrentSize() > 1))
      changeListToBit();
  }
 private:
  void changeListToBit() {
    bit_.resize(graph_size_, 0);
    for (auto it = list_.begin(); it != list_.end(); ++it)
      bit_[*it] = 1;
    list_.clear();
  }
  size_t getCurrentSize() const {
    return std::max(bit_.size(),list_.size());
  }
  bool isBit() const {
    return !bit_.empty();
  }
  size_t graph_size_;
  vector<bool> bit_;
  vector<int> list_;
};

}//namespace graph