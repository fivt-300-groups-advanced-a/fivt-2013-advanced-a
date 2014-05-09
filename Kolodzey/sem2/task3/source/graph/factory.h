#ifndef FACTORY_H
#define FACTORY_H

#include <memory>
#include <iostream>

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
                                : graph_size_(supported_size_of_graph),
                                  is_valid_(true) { }
  typedef std::unique_ptr<BaseIncidence> UPtrIncidence;
  UPtrIncidence genIncidence() {
    checkValid();
    is_valid_ = false;
    if (getCurrentSize() == 0)
      return UPtrIncidence(new EmptyIncidence());
    if (getCurrentSize() == 1)
      return UPtrIncidence(new OneVertexIncidence(list_[0]));
    else if (isBit())
      return UPtrIncidence(new AdjacencyMatrixIncidence(std::move(bit_)));
    return UPtrIncidence(new AdjacencyListIncidence(std::move(list_)));
  }
  UPtrIncidence genIncidenceSavingFactory() const {
    checkValid();
     if (getCurrentSize() == 0)
      return UPtrIncidence(new EmptyIncidence());
    if (getCurrentSize() == 1)
      return UPtrIncidence(new OneVertexIncidence(list_[0]));
    else if (isBit())
      return UPtrIncidence(new AdjacencyMatrixIncidence(bit_));
    return UPtrIncidence(new AdjacencyListIncidence(list_));
  }
  void addEdge(int v) {
    checkValid();
    if(isBit())
      bit_[v] = 1;
    else
      list_.push_back(v);
    if((!isBit()) && (getCurrentSize() > (graph_size_ / 32)) && 
                     (getCurrentSize() > 1))
      changeListToBit();
  }
  void reset(size_t supported_size_of_graph) {
    is_valid_ = true;
    graph_size_ = supported_size_of_graph;
    bit_.clear();
    list_.clear();
  }
  bool isValid() const { return is_valid_; }
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
  void checkValid() const {
    using std::cerr;
    using std::endl;
    if (!is_valid_) {
      cerr << "Call of method of invalid factory." << endl
           << "Factory becomes invalid after call of genIncidence()." << endl
           << endl
           << "To save factory state and continue working with it" << endl
           << "call genIncidenceSavingFactory()." << endl
           << endl
           << "To reset factory and build a new incidence" << endl
           << "call reset(supported_size_of_graph)." << endl;
      abort();
    }
  }
  size_t graph_size_;
  bool is_valid_;
  std::vector<bool> bit_;
  std::vector<int> list_;
};
//class GraphFactory {
//};
}//namespace graph
#endif //FACTORY_H