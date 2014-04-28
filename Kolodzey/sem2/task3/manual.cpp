#include "gtest/gtest.h"
#include "graph.h"

using graph::BaseIncidence;
using graph::BaseIterator;
using graph::AdjacencyMatrixIncidence;
using graph::AdjacencyMatrixIterator;
using graph::GraphIterator;
using graph::Graph;
using graph::Coloring;

using graph::AccessGraphIterator;
using graph::AccessAdjacencyMatrixIterator;
using graph::AccessAdjacencyMatrixIncidence;

using std::vector;
using std::unique_ptr;
using std::pair;

class graph::AccessAdjacencyMatrixIterator {
 public:
   AccessAdjacencyMatrixIterator(AdjacencyMatrixIterator* it_ptr)
       : it_ptr_(it_ptr) {}
   vector<bool>::const_iterator getPos() { return it_ptr_->pos_; }
   vector<bool>::const_iterator getEnd() { return it_ptr_->end_; }
   AdjacencyMatrixIterator* it_ptr_;
};

class graph::AccessAdjacencyMatrixIncidence {
 public:
  AccessAdjacencyMatrixIncidence(AdjacencyMatrixIncidence* li_ptr)
      : li_ptr_(li_ptr) {}
  vector<bool>& getVectorBool() { return li_ptr_->adjdata_; }
  AdjacencyMatrixIncidence* li_ptr_;
};

class graph::AccessGraphIterator {
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

TEST(Graph, isConnected) {
  bool mval [4][4] = {{0, 1, 0, 0},
                      {1, 0, 0, 1},
                      {1, 1, 0, 1},
                      {0, 0, 1, 0}};
  vector<unique_ptr<BaseIncidence>> vval;
  for (int i = 0; i < 4; ++i)
  vval.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(mval[i], mval[i] + 4)));
  Graph graph(std::move(vval));
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
  bool mval [4][4] = {{0, 1, 0, 0},
                      {1, 0, 0, 1},
                      {1, 1, 0, 1},
                      {0, 0, 1, 0}};
  vector<unique_ptr<BaseIncidence>> vval;
  for (int i = 0; i < 4; ++i)
  vval.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(mval[i], mval[i] + 4)));
  Graph graph(std::move(vval));
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

TEST(func, isPath) {
  bool m2[2][2] = {{0, 1},
                   {0, 0}};
  vector<unique_ptr<BaseIncidence>> v2;
  for (int i = 0; i < 2; ++i)
  v2.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(m2[i], m2[i] + 2)));
  Graph graph2(std::move(v2));
  EXPECT_TRUE(isPath(graph2, 0, 1));
  EXPECT_FALSE(isPath(graph2, 1, 0));

  bool m4cycle[4][4] = {{0, 1, 0, 0},
                        {0, 0, 1, 0},
                        {0, 0, 0, 1},
                        {1, 0, 0, 0}};
  vector<unique_ptr<BaseIncidence>> v4cycle;
  for (int i = 0; i < 4; ++i)
  v4cycle.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(m4cycle[i], m4cycle[i] + 4)));
  Graph graph4cycle(std::move(v4cycle));
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_TRUE(isPath(graph4cycle, i, j));
}

TEST(func, getStronglyConnectedComponentsDummy) {
  // 0 <-- 2 <-
  // |     |   \
  // v     v    \
  // 1 <-- 3 --> 4 
  // ans = 0; 1; 2 + 3 + 4 
  bool mval [5][5] = {{0, 1, 0, 0, 0},
                      {0, 0, 0, 0, 0},
                      {1, 0, 0, 1, 0},
                      {0, 1, 0, 0, 1},
                      {0, 0, 1, 0, 0}};
  vector<unique_ptr<BaseIncidence>> vval;
  for (int i = 0; i < 5; ++i)
  vval.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(mval[i], mval[i] + 5)));
  Graph graph(std::move(vval));
  Coloring components = getStronglyConnectedComponentsDummy(graph);
  EXPECT_EQ(3, components.representative.size());
  EXPECT_EQ(components.color[2], components.color[3]);
  EXPECT_EQ(components.color[2], components.color[4]);
  EXPECT_FALSE(components.color[2] == components.color[1]);
  EXPECT_FALSE(components.color[2] == components.color[0]);
  EXPECT_FALSE(components.color[1] == components.color[0]);
}

TEST(func, getSource) {

// 0 <-- 1    4    5  6
// |     ^          \ |\
// |     |           \| \
// v     |           vv v 
// 2 --> 3           7  8
//                   |
//                   v
//                   9

bool mval [10][10] = {{0, 0, 1, 0, 0,    0, 0, 0, 0, 0},
                      {1, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                      {0, 0, 0, 1, 0,    0, 0, 0, 0, 0},
                      {0, 1, 0, 0, 0,    0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                      
                      {0, 0, 0, 0, 0,    0, 0, 1, 0, 0},
                      {0, 0, 0, 0, 0,    0, 0, 1, 1, 0},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 1},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 0}};

  vector<unique_ptr<BaseIncidence>> vval;
  for (int i = 0; i < 10; ++i)
  vval.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(mval[i], mval[i] + 10)));
  Graph graph(std::move(vval));
  vector<int> source = getSource(graph);
  sort(source.begin(), source.end());
  EXPECT_EQ(2, source.size());
  EXPECT_EQ(5, source[0]);
  EXPECT_EQ(6, source[1]);
}

TEST(func, getIsSink) {

// 0 <-- 1    4    5  6
// |     ^          \ |\
// |     |           \| \
// v     |           vv v 
// 2 --> 3           7  8
//                   |
//                   v
//                   9

bool mval [10][10] = {{0, 0, 1, 0, 0,    0, 0, 0, 0, 0},
                      {1, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                      {0, 0, 0, 1, 0,    0, 0, 0, 0, 0},
                      {0, 1, 0, 0, 0,    0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                      
                      {0, 0, 0, 0, 0,    0, 0, 1, 0, 0},
                      {0, 0, 0, 0, 0,    0, 0, 1, 1, 0},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 1},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 0}};

  vector<unique_ptr<BaseIncidence>> vval;
  for (int i = 0; i < 10; ++i)
  vval.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(mval[i], mval[i] + 10)));
  Graph graph(std::move(vval));
  vector<int> sink = getSink(graph);
  sort(sink.begin(), sink.end());
  EXPECT_EQ(2, sink.size());
  EXPECT_EQ(8, sink[0]);
  EXPECT_EQ(9, sink[1]);
}

TEST(func, getIsolated) {

// 0 <-- 1    4    5  6
// |     ^          \ |\
// |     |           \| \
// v     |           vv v 
// 2 --> 3           7  8
//                   |
//                   v
//                   9

bool mval [10][10] = {{0, 0, 1, 0, 0,    0, 0, 0, 0, 0},
                      {1, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                      {0, 0, 0, 1, 0,    0, 0, 0, 0, 0},
                      {0, 1, 0, 0, 0,    0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                      
                      {0, 0, 0, 0, 0,    0, 0, 1, 0, 0},
                      {0, 0, 0, 0, 0,    0, 0, 1, 1, 0},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 1},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 0},
                      {0, 0, 0, 0, 0,    0, 0, 0, 0, 0}};

  vector<unique_ptr<BaseIncidence>> vval;
  for (int i = 0; i < 10; ++i)
  vval.emplace_back(new AdjacencyMatrixIncidence(
                              vector<bool>(mval[i], mval[i] + 10)));
  Graph graph(std::move(vval));
  vector<int> isolated = getIsolated(graph);
  sort(isolated.begin(), isolated.end());
  EXPECT_EQ(1, isolated.size());
  EXPECT_EQ(4, isolated[0]);
}

