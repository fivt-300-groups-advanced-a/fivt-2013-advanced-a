#ifndef GRAPH
#define GRAPH

#include <iostream>
#include <vector>

namespace graph {

using std::unique_ptr;
using std::vector;
using std::cerr;
using std::endl;
using std::pair;
using std::tuple;

//----Classes----//
//===============//

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

/* 
class AdjacencyListIterator : public BaseIterator {
 public: 
  override void moveForvard();
  override int get() const;
  override bool isValid() const;
  ~BaseIterator();
};

class AdjacencyListIncidence : public BaseIncidence {
 public:
  override unique_ptr<BaseIterator> begin() const;
  override int outdegree() const;
  override bool isConnected(int v) const;
  override ~VertexList();
};
*/

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

struct Coloring {
 public:
  vector<int> color;
  vector<int> delegate;
  int getColorOf(int vertex_id) const { return color[vertex_id]; }
  int getRepresentativeOf(int color_id) const { return delegate[color_id]; }
  int getNumberOfColors() const { return delegate.size(); }
  int getNumberOfVertexes() const { return color.size(); }
};


//----Algorithms to work with graph. Declared in func.cpp----//
//===========================================================//
/**
* getStronglyConnectedComponentsDummy O((V^2)(E + V))
*/
Coloring getStronglyConnectedComponentsDummy(const Graph& graph);

vector<int> getSource(const Graph& graph);
vector<int> getSink(const Graph& graph);
vector<int> getIsolated(const Graph& graph);
bool hasSelfLoop(const Graph& graph);
/**true if there is any loop in graph, including self-loop*/
bool hasLoop(const Graph& graph);
/**
* getStronglyConnectedComponentsTarjan (with stack) O(E + V)
*/
//Coloring getStronglyConnectedComponentsTarjan(const Graph& graph);
/**
* getCompletionToStrongСonnectivity O(E + V)
* launches condensation and getCompletionToStrongСonnectivityInСondensed
* returns needed egdes first -> last
*/
//vector<pair<int,int>> getCompletionToStrongСonnectivity(const Graph& graph);
/**
* getCompletionToStrongСonnectivityInСondensed O(E + V)
* is launched by getCompletionToStrongСonnectivity
* uses Tarjan algorithm
*/
vector<pair<int,int>> getCompletionToStrongСonnectivityInСondensed(
                                                         const Graph& graph); 
/**
* checks whether is Path from start to finish using dfs
*/
bool isPath(const Graph& graph, int start_id, int finish_id); //O(E + V)


//-----Fabrics, Builders, etc. Declared in fabric.cpp----//
//=======================================================//
Graph buildSimpleAdjacencyMatrix(const vector<vector<bool>>& matrix);
}//namespace graph
#endif