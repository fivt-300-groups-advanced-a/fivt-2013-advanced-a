#ifndef IMPLEMENTATIONS_H
#define IMPLEMENTATIONS_H

#include "graph.h"

namespace graph {
// AdjacencyMatrix //
// --------------- //
namespace testtools {
class AccessAdjacencyMatrixIterator;
}//namespace testools
class AdjacencyMatrixIterator : public BaseIterator {
 friend class testtools::AccessAdjacencyMatrixIterator;
 public:
  AdjacencyMatrixIterator(int vertex_id,
                          const std::vector<bool>::const_iterator pos,
                          const std::vector<bool>::const_iterator end)
      : vertex_id_(vertex_id),
        pos_(pos),
        end_(end){}
  virtual void moveForvard() override {
    if (isValid()) {
      do {
        ++pos_;
        ++vertex_id_;
      } while (isValid() && (*pos_ != true));
    }
  }
  virtual int get() const override {
    if (isValid())
      return vertex_id_;
    return -1;
  }
  virtual bool isValid() const override { return pos_ != end_; } 
  virtual ~AdjacencyMatrixIterator() {}
 private:
  int vertex_id_;
  std::vector<bool>::const_iterator pos_;
  std::vector<bool>::const_iterator end_;
};
namespace testtools {
class AccessAdjacencyMatrixIncidence;
}//namespace testtools
class AdjacencyMatrixIncidence : public BaseIncidence {
 friend class testtools::AccessAdjacencyMatrixIncidence;
 public:
  explicit AdjacencyMatrixIncidence(const std::vector<bool>& adjdata)
               : adjdata_(adjdata) { }
  explicit AdjacencyMatrixIncidence(std::vector<bool>&& adjdata)
               : adjdata_(std::move(adjdata)) { }
  virtual std::unique_ptr<BaseIterator> begin() const override {
    std::unique_ptr<BaseIterator> ptr(new AdjacencyMatrixIterator(0,
                                                             adjdata_.cbegin(),
                                                             adjdata_.cend()));
    if (adjdata_[0] != true)
      ptr->moveForvard();
    return std::move(ptr);
  }
  bool isConnected(int v) const override { return adjdata_[v]; }
  virtual ~AdjacencyMatrixIncidence() {}
 private:
  std::vector<bool> adjdata_;
};
// AdjacencyList //
// ------------- //
namespace testtools {
class AccessAdjacencyListIterator;
}//namespace testtools
class AdjacencyListIterator : public BaseIterator {
 friend class testtools::AccessAdjacencyListIterator;
 public:
  AdjacencyListIterator(std::vector<int>::const_iterator pos,
                        std::vector<int>::const_iterator end) : pos_(pos),
                                                                end_(end) {}
  void moveForvard() override {
    if (isValid())
      ++pos_;
  }
  int get() const override { 
    if (isValid())
      return *pos_;
    return -1;
  }
  bool isValid() const override { return (pos_ != end_); }
  virtual ~AdjacencyListIterator(){}
 private:
  std::vector<int>::const_iterator pos_;
  std::vector<int>::const_iterator end_;
};
namespace testtools {
class AccessAdjacencyListIncidence;
}//namespace testtools
class AdjacencyListIncidence : public BaseIncidence {
 friend class testtools::AccessAdjacencyListIncidence;
 public:
  explicit AdjacencyListIncidence(const std::vector<int>& adjdata)
                                                        : adjdata_(adjdata) {
    using std::sort;
    using std::unique;
    sort(adjdata_.begin(), adjdata_.end());
    adjdata_.erase(unique(adjdata_.begin(), adjdata_.end()), adjdata_.end());
  }
  explicit AdjacencyListIncidence(std::vector<int>&& adjdata)
                                              : adjdata_(std::move(adjdata)) {
    using std::sort;
    using std::unique;
    sort(adjdata_.begin(), adjdata_.end());
    adjdata_.erase(unique(adjdata_.begin(), adjdata_.end()), adjdata_.end());
  }
  std::unique_ptr<BaseIterator> begin() const override {
    return std::unique_ptr<BaseIterator>(new AdjacencyListIterator
                                          (adjdata_.begin(), adjdata_.end()));
  }
  virtual bool isConnected(int v) const override {
    int l = 0;
    int r = adjdata_.size();
    while ((r - l) > 1) {
      int m = (l + r) / 2;
      if (adjdata_[m] <= v)
        l = m;
      else
        r = m;
    }
    return (v == adjdata_[l]);
  }
  virtual ~AdjacencyListIncidence() {}
 private:
  std::vector<int> adjdata_;
};
// OneVertex //
// --------- //
class OneVertexIterator : public BaseIterator {
 public:
  OneVertexIterator(int vertex_id) : is_valid_(1), vertex_id_(vertex_id) {}
  void moveForvard() override { is_valid_ = 0; }
  int get() const override {
    if (isValid())
      return vertex_id_;
    return -1;
  }
  bool isValid() const override { return is_valid_; }
  virtual ~OneVertexIterator() {}
 private:
  bool is_valid_;
  int vertex_id_;
};
class OneVertexIncidence : public BaseIncidence {
 public:
  OneVertexIncidence(int vertex_id) : vertex_id_(vertex_id) {}
  std::unique_ptr<BaseIterator> begin() const override {
    return std::unique_ptr<BaseIterator>(new OneVertexIterator(vertex_id_));
  }
  bool isConnected(int v) const override { return (v == vertex_id_); }
  virtual ~OneVertexIncidence() {}
 private:
  int vertex_id_;
};
// EmptyList //
// --------- //
class EmptyIterator : public BaseIterator {
 public:
  EmptyIterator() {}
  void moveForvard() override {}
  int get() const override { return -1; }
  bool isValid() const { return false; }
  virtual ~EmptyIterator() {}
};
class EmptyIncidence : public BaseIncidence {
 public:
  std::unique_ptr<BaseIterator> begin() const override {
    return std::unique_ptr<BaseIterator>(new EmptyIterator());
  }
  bool isConnected(int /*v*/) const override { return false; }
  virtual ~EmptyIncidence() {}
};
}//namespace graph
#endif //IMPLEMENTATIONS_H