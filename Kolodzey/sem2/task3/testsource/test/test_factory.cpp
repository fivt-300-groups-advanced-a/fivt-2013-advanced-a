#include "gtest/gtest.h"
#include "factory.h"
#include "implementations.h"

using namespace std;
using namespace graph;
using namespace graph::testtools;
using namespace testing;

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
//  Manual tests of inner structure  //
//  -------------------------------  //
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
//  DeathTests, behavior after genIncidence  //
//  ---------------------------------------  //
TEST(IncidenceFactory, genIncidence) {
  IncidenceFactory factory(10);
  AccessIncidenceFactory acc_f(&factory);
  
  factory.addEdge(3);
  factory.addEdge(4);
  unique_ptr<BaseIncidence> li = factory.genIncidenceSavingFactory();
  EXPECT_EQ(true, li->isConnected(3));
  EXPECT_EQ(true, li->isConnected(4));
  EXPECT_EQ(false, li->isConnected(5));
  EXPECT_TRUE(factory.isValid());
  
  factory.addEdge(5);
  li = factory.genIncidence();
  EXPECT_EQ(true, li->isConnected(3));
  EXPECT_EQ(true, li->isConnected(4));
  EXPECT_EQ(true, li->isConnected(5));
  
  EXPECT_EQ(vector<bool>(), acc_f.getBit());
  EXPECT_EQ(vector<int>(), acc_f.getList());
  EXPECT_FALSE(factory.isValid());

  EXPECT_DEATH({ factory.genIncidence(); }, "");
  EXPECT_DEATH({ factory.genIncidenceSavingFactory(); }, "");
  EXPECT_DEATH({ factory.addEdge(3); }, "");
  
  factory.reset(6);
  EXPECT_TRUE(factory.isValid());
  EXPECT_EQ(6, acc_f.getGraphSize());
  EXPECT_EQ(vector<bool>(), acc_f.getBit());
  EXPECT_EQ(vector<int>(), acc_f.getList());
}

//  Test correctness and choise of implementation  //
//  ---------------------------------------------  //
enum TypeOfIncidence {
  EMPTY_INCIDENCE,
  ONE_VERTEX_INCIDENCE,
  ADJACENCY_LIST_INCIDENCE,
  ADJACENCY_MATRIX_INCIDENCE,
  BASE_INCIDENCE
};
TypeOfIncidence getTypeOfIncidence(BaseIncidence* ptr) {
  if (dynamic_cast<EmptyIncidence*>(ptr) != nullptr)
    return EMPTY_INCIDENCE;
  if (dynamic_cast<OneVertexIncidence*>(ptr) != nullptr)
    return ONE_VERTEX_INCIDENCE;
  if (dynamic_cast<AdjacencyListIncidence*>(ptr) != nullptr)
    return ADJACENCY_LIST_INCIDENCE;
  if (dynamic_cast<AdjacencyMatrixIncidence*>(ptr) != nullptr)
    return ADJACENCY_MATRIX_INCIDENCE;
  return BASE_INCIDENCE;
}

typedef tuple<TypeOfIncidence, size_t, vector<size_t>> ImplementParam;
typedef ::testing::TestWithParam<ImplementParam> choiseOfImplementation;

TEST_P(choiseOfImplementation, CheckChoiseAndCorrectness) {
  //extract data from param
  auto param = GetParam();
  TypeOfIncidence expected_type = get<0>(param);
  size_t size = get<1>(param);
  vector<size_t> edges = get<2>(param);
  //construct graph using factory
  IncidenceFactory factory(size);
  for (auto it = edges.begin(); it != edges.end(); ++it)
    factory.addEdge(*it);
  unique_ptr<BaseIncidence> uptr_li = factory.genIncidence();
  //check iterator
  {
    int i = 0;
    for (auto it = uptr_li->begin(); it->isValid(); it->moveForvard()) {
      EXPECT_EQ(edges[i], it->get());
      ++i;
    }
  }
  //check isConnected
  {
    size_t j = 0;
    for (size_t i = 0; i < size; ++i) {
      if (edges.empty())
        EXPECT_FALSE(uptr_li->isConnected(i));
      else {
        while ((edges[j] < i) && (j < (edges.size() - 1)))
          ++j;
        EXPECT_EQ(i == edges[j], uptr_li->isConnected(i));
      }
    }
  }
  //check type of choosen implementation
  EXPECT_EQ(expected_type, getTypeOfIncidence(uptr_li.get()));
}
INSTANTIATE_TEST_CASE_P(IncidenceFactory, choiseOfImplementation,
           /*            expected type of incidence | size | edges  */
    Values(ImplementParam(ADJACENCY_LIST_INCIDENCE,    100, {3, 5, 7}),
           ImplementParam(ADJACENCY_MATRIX_INCIDENCE,    8, {3, 5, 7}),
           ImplementParam(ONE_VERTEX_INCIDENCE,        100, {3}),
           ImplementParam(ONE_VERTEX_INCIDENCE,          1, {3}),
           ImplementParam(EMPTY_INCIDENCE,             100, {}),
           ImplementParam(EMPTY_INCIDENCE,               0, {})));


//  Testing GraphFactory  //
//  ====================  //
TEST(GraphFactory, Manual) {
  GraphFactory factory(5);
  EXPECT_TRUE(factory.isValid());

  factory.addEdge(3, 4);
  factory.addEdge(2, 3);
  Graph g(factory.genGraphSavingFactory());

  EXPECT_TRUE(g.isConnected(3, 4));
  EXPECT_FALSE(g.isConnected(4, 3));
  EXPECT_TRUE(g.isConnected(2, 3));
  EXPECT_FALSE(g.isConnected(3, 2));
  EXPECT_FALSE(g.isConnected(0, 1));
  EXPECT_FALSE(g.isConnected(1, 0));
  
  EXPECT_TRUE(factory.isValid());

  factory.addEdge(0, 1);
  g = factory.genGraph();

  EXPECT_TRUE(g.isConnected(3, 4));
  EXPECT_FALSE(g.isConnected(4, 3));
  EXPECT_TRUE(g.isConnected(2, 3));
  EXPECT_FALSE(g.isConnected(3, 2));
  EXPECT_TRUE(g.isConnected(0, 1));
  EXPECT_FALSE(g.isConnected(1, 0));
  
  EXPECT_FALSE(factory.isValid());
  EXPECT_DEATH({ factory.addEdge(3, 2); }, "");
  EXPECT_DEATH({ factory.genGraph(); }, "");
  EXPECT_DEATH({ factory.genGraphSavingFactory(); }, "");

  factory.reset(6);
  g = factory.genGraph();
  for (int i = 0; i < 6; ++i)
    for (int j = 0; j < 6; ++j)
      EXPECT_FALSE(g.isConnected(i, j));
}

