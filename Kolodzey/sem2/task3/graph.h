#include <iostream>
#include <vector>

namespace graph {
using std::unique_ptr;
using vector;

class BaseIterator {
 public:
  virtual void moveForvard() = delete;
  virtual int getValue() const = delete;
  virtual bool isValid() const = delete;
  virtual ~BaseIterator();
};

class BaseIncidence {
 public:
  virtual unique_ptr<BaseIterator> begin() const = delete;
  virtual int outdegree() const = delete;
  virtual bool isConnected(int v) const = delete;
  virtual ~VertexList();
};

class AdjacencyMatrixIterator :: public BaseIterator {
 public:
  AdjacencyMatrixIterator(vector<bool>::iterator pos,
                          int val,
                          vector<bool>::iterator end,) : pos_(pos),
                                                         val_(val), 
                                                         end_(end) {}
  override void moveForvard() {
    do {
      ++pos_;
      ++val_;
    } while (isValid() && (*pos_ != true));
  }
  override int getValue() const { return val_; }
  override bool isValid() const { return pos_ != end_; }
  virtual ~BaseIterator();
 private:
  vector<bool>::iterator pos_;
  vector<bool>::iterator end_;
  int val_;
};

class AdjacencyMatrixIncidence :: public BaseIncidence {
 public:
  AdjacencyMatrixIncidence(vector<bool> is_adjacent_to):
                                        is_adjacent_to_(is_adjacent_to) {
    outdegree_ = 0;
    vector<bool>::iterator it;
    for (it = is_adjacent_to_.begin(); it != is_adjacent_to_.end(); ++it) {
      if (*it == true)
        ++outdegree_;
    }
  }
  override unique_ptr<BaseIterator> begin() const {
    return std::move(unique_ptr
               (new AdjacencyMatrixIterator(is_adjacent_to_.begin(), 0,
                                            is_adjacent_to_.end())));
  }
  override int outdegree() const { return outdegree_; }
  override bool isConnected(int v) const { return is_adjacent_to_[v]; }
  virtual ~VertexList();
 private:
  vector<bool> is_adjacent_to_;
  int outdegree_;
};

/*
class AdjacencyListIterator :: public BaseIterator {
 public: 
  override void moveForvard();
  override int getValue() const;
  override bool isValid() const;
  ~BaseIterator();
};

class AdjacencyListIncidence :: public BaseIncidence {
 public:
  override unique_ptr<BaseIterator> begin() const;
  override int outdegree() const;
  override bool isConnected(int v) const;
  override ~VertexList();
};
*/
}//namespace graph
