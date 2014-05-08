#include "gtest/gtest.h"
#include "graph.h"
#include "implementations.h"

using namespace graph;
using namespace graph::testtools;
using namespace std;

//  AdjacencyMatrixIterator  //
//  -----------------------  //
namespace graph {
namespace testtools {
class AccessAdjacencyMatrixIterator {
 public:
   AccessAdjacencyMatrixIterator(AdjacencyMatrixIterator* it_ptr)
       : it_ptr_(it_ptr) {}
   vector<bool>::const_iterator getPos() { return it_ptr_->pos_; }
   vector<bool>::const_iterator getEnd() { return it_ptr_->end_; }
   AdjacencyMatrixIterator* it_ptr_;
};
}//namespace testtools
}//namespace graph
TEST(AdjacencyMatrixIterator, Constructor) {
  vector<bool> values = {0, 0, 1, 1, 0, 1};

  AdjacencyMatrixIterator it(0, values.begin(), values.end());
  
  AccessAdjacencyMatrixIterator acc_it(&it);
  EXPECT_EQ(values.begin(), acc_it.getPos());
  EXPECT_EQ(values.end(), acc_it.getEnd());
  EXPECT_EQ(0, it.get());
}
TEST(AdjacencyMatrixIterator, get) {
  vector<bool> values = {0, 0, 1, 1, 0, 1};
  
  AdjacencyMatrixIterator it0(0, values.begin(), values.end());
  EXPECT_EQ(0, it0.get());
  AdjacencyMatrixIterator it3(3, values.begin(), values.end());
  EXPECT_EQ(3, it3.get());
}
TEST(AdjacencyMatrixIterator, isValid) {
  vector<bool> values = {0, 0, 1, 1, 0, 1};
  
  AdjacencyMatrixIterator it0(0, values.begin(), values.end());
  EXPECT_EQ(true, it0.isValid());
  AdjacencyMatrixIterator it3(3, values.end(), values.end());
  EXPECT_EQ(false, it3.isValid());
}
TEST(AdjacencyMatrixIterator, moveForvard) {
  vector<bool> values = {0, 0, 1, 1, 0, 1};  
  AdjacencyMatrixIterator matrix_it(2, values.begin() + 2, values.end());
  AccessAdjacencyMatrixIterator acc_matrix_it(&matrix_it);
  int val = 0;

  for (vector<bool>::iterator it = values.begin(); it != values.end(); ++it) {
    if (*it == true) {
      EXPECT_EQ(val, matrix_it.get());
      EXPECT_EQ(it, acc_matrix_it.getPos());
      EXPECT_EQ(values.end(), acc_matrix_it.getEnd());
      matrix_it.moveForvard();
    }
    ++val;
  }
  EXPECT_EQ(-1, matrix_it.get());
  EXPECT_EQ(values.end(), acc_matrix_it.getPos());
  EXPECT_EQ(values.end(), acc_matrix_it.getEnd());
  matrix_it.moveForvard();
  EXPECT_EQ(-1, matrix_it.get());
  EXPECT_EQ(values.end(), acc_matrix_it.getPos());
  EXPECT_EQ(values.end(), acc_matrix_it.getEnd());
}
//  AdjacencyMatrixIncidence  //
//  ------------------------  //
class graph::testtools::AccessAdjacencyMatrixIncidence {
 public:
  AccessAdjacencyMatrixIncidence(AdjacencyMatrixIncidence* li_ptr)
      : li_ptr_(li_ptr) {}
  vector<bool>& getVectorBool() { return li_ptr_->adjdata_; }
  AdjacencyMatrixIncidence* li_ptr_;
};
TEST(AdjacencyMatrixIncidence, Constructor) {
  bool init[6] = {0, 0, 1, 1, 0, 1};
  vector<bool> values(init, init + 6);
  //Constructor which copies vector<bool>
  AdjacencyMatrixIncidence li1(values);
  AccessAdjacencyMatrixIncidence access_li(&li1);
  EXPECT_EQ(vector<bool>(init, init + 6), access_li.getVectorBool());
  EXPECT_EQ(vector<bool>(init, init + 6), values);

  //Constructor which moves vector<bool>
  AdjacencyMatrixIncidence li2(std::move(values));
  access_li.li_ptr_ = &li2;
  EXPECT_EQ(vector<bool>(init, init + 6), access_li.getVectorBool());
  EXPECT_EQ(vector<bool>(), values);
}
TEST(AdjacencyMatrixIncidence, Begin) {
  bool init[6] = {0, 0, 1, 1, 0, 1};

  AdjacencyMatrixIncidence li1(vector<bool>(init, init + 6));
  std::unique_ptr<BaseIterator> ptr_it1 = li1.begin();
  EXPECT_EQ(2, ptr_it1->get());
  AdjacencyMatrixIncidence li2(vector<bool>(init + 2, init + 6));
  std::unique_ptr<BaseIterator> ptr_it2 = li2.begin();
  EXPECT_EQ(0, ptr_it2->get());
}
TEST(AdjacencyMatrixIncidence, isConnected) {
  vector<bool> values = {0, 0, 1, 0, 1, 0, 1, 0};
  AdjacencyMatrixIncidence li(values);
  for (int i = 0; i < (int)(values.size()); ++i)
    EXPECT_EQ(values[i], li.isConnected(i));
}
//  AdjacencyListIterator  //
//  ---------------------  //
class graph::testtools::AccessAdjacencyListIterator {
 public:
  AccessAdjacencyListIterator(AdjacencyListIterator* it_ptr)
       : it_ptr_(it_ptr) {}
   vector<int>::const_iterator getPos() { return it_ptr_->pos_; }
   vector<int>::const_iterator getEnd() { return it_ptr_->end_; }
   AdjacencyListIterator* it_ptr_;
};
TEST(AdjacencyListIterator, Constructor) {
  vector<int> values = {1, 2, 3, 5, 0, 7};

  AdjacencyListIterator it(values.begin(), values.end());
  
  AccessAdjacencyListIterator acc_it(&it);
  EXPECT_EQ(values.begin(), acc_it.getPos());
  EXPECT_EQ(values.end(), acc_it.getEnd());
}
TEST(AdjacencyListIterator, get) {
  vector<int> values = {1, 2, 3, 5, 0, 7};
  for (auto it = values.cbegin(); it != values.cend(); ++it) {
    AdjacencyListIterator listIt(it, values.cend());
    EXPECT_EQ(*it, listIt.get());
  }
  AdjacencyListIterator listIt(values.cend(), values.cend());
  EXPECT_EQ(-1, listIt.get());
}
TEST(AdjacencyListIterator, isValid) {
  vector<int> values = {1, 2, 3, 5, 0, 7};
  for (auto it = values.cbegin(); it != values.cend(); ++it) {
    AdjacencyListIterator listIt(it, values.cend());
    EXPECT_EQ(true, listIt.isValid());
  }
  AdjacencyListIterator listIt(values.cend(), values.cend());
  EXPECT_EQ(false, listIt.isValid());
}
TEST(AdjacencyListIterator, moveForvard) {
  vector<int> values = {1, 2, 3, 5, 0, 7};
  AdjacencyListIterator it(values.begin(), values.cend());
  AccessAdjacencyListIterator acc_it(&it);
  for (size_t i = 0; i < values.size(); ++i) {
    EXPECT_EQ(values.begin() + i, acc_it.getPos());
    EXPECT_EQ(values.end(), acc_it.getEnd());
    EXPECT_EQ(values[i], it.get());
    it.moveForvard();
  }
  EXPECT_EQ(values.end(), acc_it.getPos());
  EXPECT_EQ(values.end(), acc_it.getEnd());
  EXPECT_EQ(-1, it.get());
  it.moveForvard();
  EXPECT_EQ(values.end(), acc_it.getPos());
  EXPECT_EQ(values.end(), acc_it.getEnd());
  EXPECT_EQ(-1, it.get());
}
//  AdjacencyListIncidence  //
//  ----------------------  //
class graph::testtools::AccessAdjacencyListIncidence {
 public:
  AccessAdjacencyListIncidence(AdjacencyListIncidence* li_ptr)
      : li_ptr_(li_ptr) {}
  vector<int>& getVectorInt() { return li_ptr_->adjdata_; }

  AdjacencyListIncidence* li_ptr_;
};
TEST(AdjacencyListIncidence, Constructor) {
  int init[6] = {0, 5, 7, 8, 10, 11};
  vector<int> values(init, init + 6);
  //Constructor which copies vector<bool>
  AdjacencyListIncidence li1(values);
  AccessAdjacencyListIncidence access_li(&li1);
  EXPECT_EQ(vector<int>(init, init + 6), access_li.getVectorInt());
  EXPECT_EQ(vector<int>(init, init + 6), values);

  //Constructor which moves vector<bool>
  AdjacencyListIncidence li2(std::move(values));
  access_li.li_ptr_ = &li2;
  EXPECT_EQ(vector<int>(init, init + 6), access_li.getVectorInt());
  EXPECT_EQ(vector<int>(), values);
}
TEST(AdjacencyListIncidence, Begin) {
  vector<int> data = {1, 3, 5, 7};
  AdjacencyListIncidence li(data);
  auto it = li.begin();
  EXPECT_EQ(1, it->get());
  it->moveForvard();
  EXPECT_EQ(3, it->get());
}
TEST(AdjacencyListIncidence, isConnected) {
  vector<int> data = {1, 3, 5, 7};
  AdjacencyListIncidence li(data);
  auto it = data.begin();
  for (int i = 0; i <= 8; ++i) {
    while ((*it) < i)
      ++it;
    EXPECT_EQ(i == (*it), li.isConnected(i)) << i;
  }
}
// OneVertexIterator  //
// -----------------  //
TEST(OneVertexIterator, AllExeptMoveForvard) {
  OneVertexIterator it(3);
  EXPECT_EQ(true, it.isValid());
  EXPECT_EQ(3, it.get());
}
TEST(OneVertexIterator, MoveForvard) {
  OneVertexIterator it(3);
  it.moveForvard();
  EXPECT_EQ(false, it.isValid());
  EXPECT_EQ(-1, it.get());
}
//  OneVertexIncidence  //
//  ------------------  //
TEST(OneVertexIncidence, Manual) {
  OneVertexIncidence li(4);
  for (int i = 0; i < 6; ++i)
    EXPECT_EQ((i == 4), li.isConnected(i));
  auto it = li.begin();
  EXPECT_EQ(true, it->isValid());
  EXPECT_EQ(4, it->get());
}
// EmtyIterator //
// ------------ //
TEST(EmptyIterator, Manual) {
  EmptyIterator it;
  EXPECT_EQ(-1, it.get());
  EXPECT_EQ(false, it.isValid());
  it.moveForvard();
  EXPECT_EQ(-1, it.get());
  EXPECT_EQ(false, it.isValid());
}
// EmptyIncidence //
// -------------- //
TEST(EmptyIncidence, Manual) {
  EmptyIncidence li;
  for (int i = 0; i < 5; ++i)
    EXPECT_FALSE(li.isConnected(i));
  auto it = li.begin();
  EXPECT_EQ(-1, it->get());
  EXPECT_EQ(false, it->isValid());
}