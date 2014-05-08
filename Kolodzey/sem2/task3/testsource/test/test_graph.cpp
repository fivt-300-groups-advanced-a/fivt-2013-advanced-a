#include <tuple>

#include "gtest/gtest.h"
#include "graph.h"
#include "gen.h"

using namespace graph;
using namespace graph::testtools;
using namespace std;

//  GraphIterator  //
//  -------------  //
namespace graph {
namespace testtools {
class AccessGraphIterator {
 public:
  AccessGraphIterator(GraphIterator* it_ptr) : it_ptr_(it_ptr) {}
  vector<unique_ptr<BaseIncidence>>::const_iterator getPos() {
    return it_ptr_->pos_;
  }
  vector<unique_ptr<BaseIncidence>>::const_iterator getEnd() {
    return it_ptr_->end_;
  }
  GraphIterator* it_ptr_;
};
}//namespace testtools
}//namespace graph
TEST(GraphIterator, Constructor) {
  vector<unique_ptr<BaseIncidence>> values(10);
  GraphIterator it(0, values.begin(), values.end());
  AccessGraphIterator acc_it(&it);
  EXPECT_EQ(values.begin(), acc_it.getPos());
  EXPECT_EQ(values.end(), acc_it.getEnd());
  EXPECT_EQ(0, it.get());
}
TEST(GraphIterator, get) {
  vector<unique_ptr<BaseIncidence>> values(10);
  
  GraphIterator it0(0, values.begin(), values.end());
  EXPECT_EQ(0, it0.get());
  GraphIterator it3(3, values.begin(), values.end());
  EXPECT_EQ(3, it3.get());
}
TEST(GraphIterator, isValid) {
  vector<unique_ptr<BaseIncidence>> values(10);
  
  GraphIterator it0(0, values.begin(), values.end());
  EXPECT_EQ(true, it0.isValid());
  GraphIterator it3(3, values.end(), values.end());
  EXPECT_EQ(false, it3.isValid());
}
TEST(GraphIterator, moveForvard) {
  vector<unique_ptr<BaseIncidence>> values(10); 
  GraphIterator it(0, values.begin(), values.end());
  AccessGraphIterator acc_it(&it);
  vector<unique_ptr<BaseIncidence>>::const_iterator v_it = values.begin();
  for (int i = 0; i < 10; ++i, ++v_it) {
    EXPECT_EQ(i, it.get());
    EXPECT_EQ(v_it, acc_it.getPos());
    EXPECT_EQ(values.end(), acc_it.getEnd());
    EXPECT_EQ(true, it.isValid());
    it.moveForvard();
  }
    EXPECT_EQ(-1, it.get());
    EXPECT_EQ(values.end(), acc_it.getPos());
    EXPECT_EQ(values.end(), acc_it.getEnd());
    EXPECT_EQ(false, it.isValid());
    it.moveForvard();
    EXPECT_EQ(-1, it.get());
    EXPECT_EQ(values.end(), acc_it.getPos());
    EXPECT_EQ(values.end(), acc_it.getEnd());
    EXPECT_EQ(false, it.isValid());
}
//  Graph  //
//  -----  //
TEST(Graph, isConnected) {
  vector<vector<bool>> mval = {{0, 1, 0, 0},
                               {1, 0, 0, 1},
                               {1, 1, 0, 1},
                               {0, 0, 1, 0}};
  Graph graph = buildSimpleAdjacencyMatrix(mval);
  for(int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      EXPECT_EQ(mval[i][j], graph.isConnected(i, j));
    }
  }
  EXPECT_DEATH({ graph.isConnected(5, 0); }, "");
  EXPECT_DEATH({ graph.isConnected(-1, 0); }, "");
  EXPECT_DEATH({ graph.isConnected(1, 5); }, "");
  EXPECT_DEATH({ graph.isConnected(2, -1); }, "");
}
TEST(Graph, Begin) {
  vector<vector<bool>> mval = {{0, 1, 0, 0},
                               {1, 0, 0, 1},
                               {1, 1, 0, 1},
                               {0, 0, 1, 0}};
  Graph graph(buildSimpleAdjacencyMatrix(mval));
  for(int i = 0; i < 4; ++i) {
    unique_ptr<BaseIterator> it = graph.begin(i);
    for(int j = 0; j < 4; ++j)
      if (mval[i][j]) {
        EXPECT_EQ(j, it->get());
        it->moveForvard();
      }
  }
  unique_ptr<BaseIterator> it = graph.begin(-1);
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(i, it->get());
    it->moveForvard();
  }
  EXPECT_DEATH({ graph.begin(5); }, "");
  EXPECT_DEATH({ graph.begin(-2); }, "");
}