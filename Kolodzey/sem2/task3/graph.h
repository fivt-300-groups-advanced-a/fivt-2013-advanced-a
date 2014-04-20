#include <iostream>

#include <vector>

namespace graph {

using std::unique_ptr;
using std::vector;
using std::cerr;

class BaseIterator {
 public:
  virtual void moveForvard() = 0;
  virtual int getCurrentVertexId() const = 0;
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

  virtual int getCurrentVertexId() const override {
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

class
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
  override int getCurrentVertexId() const;
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


class Graph {
 public:
  Graph(vector<unique_ptr<BaseIncidence>> &&incidence)
      : incidence_(std::move(incidence)) { }
  unique_ptr<BaseIterator> begin(int vertex_id) const {
    if ((vertex_id >= incidence_.size()) || (vertex_id < -1)) {
      cerr << "impossible to return begin(" << vertex_id << ")" << std::endl;
      cerr << "Id out of range" << std::endl;
      abort();
    }
    //if (vertex_id == -1)
    //  return Iterator trough all vertexes
    return ;
  }
  bool isConnected(int u, int v)
 private:
  vector<unique_ptr<BaseIncidence>> incidence_;
};

}//namespace graph
