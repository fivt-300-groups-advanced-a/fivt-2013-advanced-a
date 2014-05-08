#include "gtest/gtest.h"
#include "factory.h"
#include "implementations.h"

using namespace std;
using namespace graph;
using namespace graph::testtools;

//  Testing IncidenceFactory  //
//  ========================  //
class graph::testtools::AccessIncidenceFactory {
 public:
  AccessIncidenceFactory(IncidenceFactory* factory_ptr)
        : factory_ptr_(factory_ptr) {}
  size_t& getGraphSize() { return factory_ptr_->graph_size_; }
  vector<bool>& getBit() { return factory_ptr_->bit_; }
  vector<int>& getList() { return factory_ptr_->list_; }
  void changeListToBit() { factory_ptr_->changeListToBit(); }
  size_t getCurrentSize() { return factory_ptr_->getCurrentSize(); }
  bool isBit() { return factory_ptr_->isBit(); }

  IncidenceFactory* factory_ptr_;
};
TEST(IncidenceFactory, Constructor) {
  IncidenceFactory factory(13);
  AccessIncidenceFactory acc_f(&factory);
  EXPECT_EQ(13, acc_f.getGraphSize());
  EXPECT_EQ(vector<bool>(), acc_f.getBit());
  EXPECT_EQ(vector<int>(), acc_f.getList());
}
TEST(IncidenceFactory, addEdge) {
  IncidenceFactory factory(100);  //32 * 3 < 100 < 32 * 4
  AccessIncidenceFactory acc_f(&factory);
  for (int i = 0; i < 3; ++i) {
    factory.addEdge(i + 5);
    EXPECT_EQ(i + 5, acc_f.getList().back());
    EXPECT_EQ(i + 1, acc_f.getList().size());
    EXPECT_TRUE(acc_f.getBit().empty());
  }
  factory.addEdge(8);
  EXPECT_TRUE(acc_f.getList().empty());
  EXPECT_EQ(acc_f.getBit().size(), 100);
  factory.addEdge(9);
  EXPECT_TRUE(acc_f.getList().empty());
  EXPECT_EQ(acc_f.getBit().size(), 100);
  for (int i = 0; i < 5; ++i)
    EXPECT_EQ(false, acc_f.getBit()[i]);
  for (int i = 5; i < 10; ++i)
    EXPECT_EQ(true, acc_f.getBit()[i]);
  for (int i = 10; i < 100; ++i)
    EXPECT_EQ(false, acc_f.getBit()[i]);
}