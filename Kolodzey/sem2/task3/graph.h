#include <iostream>

#include <vector>

namespace graph {
using std::unique_ptr;
using std::vector;

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
  AdjacencyMatrixIterator(const vector<bool>::const_iterator pos,
                          int vertex_id,
                          const vector<bool>::const_iterator end)
              : pos_(pos),
                vertex_id_(vertex_id),
                end_(end){}

  //make iterator refer to the nearest connected vertex, which doesn't
  //coincide with the current vertex whenever current is connected or not
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
  vector<bool>::const_iterator pos_;
  int vertex_id_;
  vector<bool>::const_iterator end_;
 friend class AccessAdjacencyMatrixIterator;
};

class AccessAdjacencyMatrixIncidence;

class AdjacencyMatrixIncidence : public BaseIncidence {
 public:
  explicit AdjacencyMatrixIncidence(const vector<bool>& is_adjacent_to)
                                        : is_adjacent_to_(is_adjacent_to) { }
  explicit AdjacencyMatrixIncidence(vector<bool>&& is_adjacent_to)
                             : is_adjacent_to_(std::move(is_adjacent_to)) { }

  virtual unique_ptr<BaseIterator> begin() const override {
    AdjacencyMatrixIterator it(is_adjacent_to_.cbegin(), 0,
                               is_adjacent_to_.cend());
    if (is_adjacent_to_[0] != true)
      it.moveForvard();
    unique_ptr<BaseIterator> ptr (&it);
    return std::move(ptr);
  }
  bool isConnected(int v) const override { return is_adjacent_to_[v]; }
  virtual ~AdjacencyMatrixIncidence() {}
 private:
  vector<bool> is_adjacent_to_;
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

/*
class BaseGraph {
 private:
  vector<unique_ptr<BaseIncidence>> _
};
*/
}//namespace graph
