#ifndef GRAPH_H
#define GRAPH_H

#include <memory>
#include <vector>
#include <iostream>

namespace graph {
class BaseIterator {
 public:
  virtual void moveForvard() = 0;
  virtual int get() const = 0;
  virtual bool isValid() const = 0;
  virtual ~BaseIterator() {}
};
class BaseIncidence {

 public:
  virtual std::unique_ptr<BaseIterator> begin() const = 0;
  virtual bool isConnected(int v) const = 0;
  virtual ~BaseIncidence() {}
};
namespace testtools {
class AccessGraphIterator;
}//namespace testtools
class GraphIterator : public BaseIterator {
  typedef std::vector<std::unique_ptr<BaseIncidence>>::const_iterator
                                          ConstIteratorViaPtrToBaseIncidence;
 public: 
  GraphIterator(int vertex_id,
                const ConstIteratorViaPtrToBaseIncidence pos,
                const ConstIteratorViaPtrToBaseIncidence end)
      : vertex_id_(vertex_id),
        pos_(pos),
        end_(end){}
  virtual void moveForvard() override {
    if (isValid()) {
        ++pos_;
        ++vertex_id_;
      }
  }
  virtual int get() const override {
    if (isValid())
      return vertex_id_;
    return -1;
  }
  virtual bool isValid() const override { return pos_ != end_; } 
  virtual ~GraphIterator() {}
 private:
  int vertex_id_;
  ConstIteratorViaPtrToBaseIncidence pos_;
  ConstIteratorViaPtrToBaseIncidence end_;
 friend class testtools::AccessGraphIterator;
};
class Graph {
 public:
  Graph() {}
  Graph(const Graph&) = delete;
  Graph& operator = (const Graph&) = delete;
  Graph(Graph&& graph) = default;
  Graph& operator = (Graph&&) = default;
  Graph(std::vector<std::unique_ptr<BaseIncidence>> &&incidence)
      : incidence_(move(incidence)) { }

  std::unique_ptr<BaseIterator> begin(int vertex_id) const {
    if ((vertex_id >= int(size())) || (vertex_id < -1)) {
      using std::cerr;
      using std::endl;
      cerr << "Impossible to return begin(" << vertex_id << ")" << endl;
      cerr << "Id out of range" << endl;
      cerr << "Possible ids are 0 ... " << size() - 1 << endl;
      cerr << "And -1 to get iterator via all vertexes" << endl;
      abort();
    }
    if (vertex_id == -1) //return Iterator via all vertexes
      return std::unique_ptr<GraphIterator> (new GraphIterator(0,
                                                          incidence_.cbegin(),
                                                          incidence_.cend()));
    return incidence_[vertex_id]->begin();
  }
  bool isConnected(int u, int v) const {
    if ((u >= int(size())) || (u < 0) || (v >= int(size())) || (v < 0)) {
      using std::cerr;
      using std::endl;
      cerr << "Impossible to return isConnected(" << u << ", "
                                                  << v << ")" << endl;
      cerr << "Id out of range" << endl;
      cerr << "Possible ids are 0 ... " << size() - 1 << endl;
      abort();
    }
    return incidence_[u]->isConnected(v);
  }
  size_t size() const {
    return incidence_.size();
  }

 private:
  std::vector<std::unique_ptr<BaseIncidence>> incidence_;
};
}//namespace graph
#endif //GRAPH_H