#ifndef FACTORY_H
#define FACTORY_H

#include <memory>

#include "graph.h"
#include "implementations.h"

namespace graph {
namespace testtools {
class AccessIncidenceFactory;
}//namespace testtools
class IncidenceFactory {
 friend class testtools::AccessIncidenceFactory;
 public: 
  IncidenceFactory(size_t supported_size_of_graph)
                                : graph_size_(supported_size_of_graph) { }
  std::unique_ptr<BaseIncidence> getIncidence() {
    if (getCurrentSize() == 0)
      return std::unique_ptr<BaseIncidence>(new EmptyIncidence());
    if (getCurrentSize() == 1)
      return std::unique_ptr<BaseIncidence>(new OneVertexIncidence(list_[0]));
    if (isBit())
      return std::unique_ptr<BaseIncidence>(new AdjacencyMatrixIncidence(bit_));
    return std::unique_ptr<BaseIncidence>(new AdjacencyListIncidence(list_));
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
  std::vector<bool> bit_;
  std::vector<int> list_;
};
}//namespace graph
#endif //FACTORY_H