#ifndef GRAPH
#define GRAPH

#include <iostream>
#include <vector>
#include <algorithm>

namespace graph {

using std::unique_ptr;
using std::vector;
using std::cerr;
using std::endl;
using std::pair;
using std::tuple;
using std::sort;
using std::unique;

//  Classes  //
//  =======  //
// Base Interface //
// -------------- //
class BaseIterator {
 public:
  virtual void moveForvard() = 0;
  virtual int get() const = 0;
  virtual bool isValid() const = 0;
  virtual ~BaseIterator() {}
};
class BaseIncidence {
 public:
  virtual unique_ptr<BaseIterator> begin() const = 0;
  virtual bool isConnected(int v) const = 0;
  virtual ~BaseIncidence() {}
};
// AdjacencyMatrix //
// --------------- //
class AccessAdjacencyMatrixIterator;
class AdjacencyMatrixIterator : public BaseIterator {
 
 public:
  
  AdjacencyMatrixIterator(int vertex_id,
                          const vector<bool>::const_iterator pos,
                          const vector<bool>::const_iterator end)
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
  vector<bool>::const_iterator pos_;
  vector<bool>::const_iterator end_;

 friend class AccessAdjacencyMatrixIterator;
};
class AccessAdjacencyMatrixIncidence;
class AdjacencyMatrixIncidence : public BaseIncidence {
 
 public:

  explicit AdjacencyMatrixIncidence(const vector<bool>& adjdata)
               : adjdata_(adjdata) { }

  explicit AdjacencyMatrixIncidence(vector<bool>&& adjdata)
               : adjdata_(std::move(adjdata)) { }

  virtual unique_ptr<BaseIterator> begin() const override {
    unique_ptr<BaseIterator> ptr(new AdjacencyMatrixIterator(0,
                                                             adjdata_.cbegin(),
                                                             adjdata_.cend()));
    if (adjdata_[0] != true)
      ptr->moveForvard();
    return std::move(ptr);
  }

  bool isConnected(int v) const override { return adjdata_[v]; }

  virtual ~AdjacencyMatrixIncidence() {}

 private:
  vector<bool> adjdata_;

 friend class AccessAdjacencyMatrixIncidence;
};
// AdjacencyList //
// ------------- //
class AccessAdjacencyListIterator;
class AdjacencyListIterator : public BaseIterator {
 public:
  AdjacencyListIterator(vector<int>::const_iterator pos,
                        vector<int>::const_iterator end) : pos_(pos),
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
  vector<int>::const_iterator pos_;
  vector<int>::const_iterator end_;
 friend class AccessAdjacencyListIterator;
};
class AccessAdjacencyListIncidence;
class AdjacencyListIncidence : public BaseIncidence {
 public:
  explicit AdjacencyListIncidence(const vector<int>& adjdata)
                                                   : adjdata_(adjdata) {
    sort(adjdata_.begin(), adjdata_.end());
    adjdata_.erase(unique(adjdata_.begin(), adjdata_.end()), adjdata_.end());
  }
  explicit AdjacencyListIncidence(vector<int>&& adjdata)
                                              : adjdata_(std::move(adjdata)) {
    sort(adjdata_.begin(), adjdata_.end());
    adjdata_.erase(unique(adjdata_.begin(), adjdata_.end()), adjdata_.end());
  }
  unique_ptr<BaseIterator> begin() const override {
    return unique_ptr<BaseIterator>(new AdjacencyListIterator
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
  vector<int> adjdata_;
 friend class AccessAdjacencyListIncidence;
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
// Graph //
// ----- //
class AccessGraphIterator;
class GraphIterator : public BaseIterator {

 public:
  
  GraphIterator(int vertex_id,
                const vector<unique_ptr<BaseIncidence>>::const_iterator pos,
                const vector<unique_ptr<BaseIncidence>>::const_iterator end)
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
  vector<unique_ptr<BaseIncidence>>::const_iterator pos_;
  vector<unique_ptr<BaseIncidence>>::const_iterator end_;

 friend class AccessGraphIterator;
};
class Graph {
 public:
  Graph() {}

  Graph(const Graph&) = delete;
  Graph& operator = (const Graph&) = delete;
  
  //Don't use explicit here!
  Graph(Graph&& graph) : incidence_(std::move(graph.incidence_)) {}
  Graph& operator = (Graph&& graph) {
    incidence_ = std::move(graph.incidence_);
    return *this;
  }

  Graph(vector<unique_ptr<BaseIncidence>> &&incidence)
      : incidence_(std::move(incidence)) { }

  unique_ptr<BaseIterator> begin(int vertex_id) const {
    if ((vertex_id >= int(size())) || (vertex_id < -1)) {
      cerr << "Impossible to return begin(" << vertex_id << ")" << endl;
      cerr << "Id out of range" << endl;
      cerr << "Possible ids are 0 ... " << size() - 1 << endl;
      cerr << "And -1 to get iterator via all vertexes" << endl;
      abort();
    }
    if (vertex_id == -1) //return Iterator via all vertexes
      return unique_ptr<GraphIterator> (new GraphIterator(0,
                                                          incidence_.cbegin(),
                                                          incidence_.cend()));
    return incidence_[vertex_id]->begin();
  }

  bool isConnected(int u, int v) const {
    if ((u >= int(size())) || (u < 0) || (v >= int(size())) || (v < 0)) {
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
  vector<unique_ptr<BaseIncidence>> incidence_;
};
// Other structures //
// ---------------- //
struct Coloring {
 public:
  vector<int> color;
  vector<int> delegate;
  int getColorOf(int vertex_id) const { return color[vertex_id]; }
  int getRepresentativeOf(int color_id) const { return delegate[color_id]; }
  int getNumberOfColors() const { return delegate.size(); }
  int getNumberOfVertexes() const { return color.size(); }
};

//  Algorithms to work with graph. Declared in func.cpp  //
//  ===================================================  //
vector<int> getSource(const Graph& graph);
vector<int> getSink(const Graph& graph);
vector<int> getIsolated(const Graph& graph);
bool hasLoop(const Graph& graph); //even for self-loop
vector<pair<int,int>> getCompletionToStrongСonnectivityInСondensed(
                                                         const Graph& graph); 

//  Factories, Builders, etc. Declared in factory.cpp  //
//  ================================================  //
}//namespace graph
#endif