#include <string>
#include "gtest/gtest.h"
#include "generaltree.h"
#include "access.h"

//---Testing index functions---
TEST(IndexFunctions, sizeOfTree)
{
  //counting number of elements in tree by length of segment
  EXPECT_EQ(1, segmentTree::sizeOfTree(1));
  EXPECT_EQ(3, segmentTree::sizeOfTree(2));
  EXPECT_EQ(5, segmentTree::sizeOfTree(3));
  EXPECT_EQ(19, segmentTree::sizeOfTree(10));
}

TEST(IndexFunctions, lastMetaVertexIndex)
{
  //counting lastMetaVertexIndex by treesize
  EXPECT_EQ((size_t)(-1), segmentTree::lastMetaVertexIndex(1));
  EXPECT_EQ(0, segmentTree::lastMetaVertexIndex(3));
  EXPECT_EQ(1, segmentTree::lastMetaVertexIndex(5));
  EXPECT_EQ(8, segmentTree::lastMetaVertexIndex(19));
}

TEST(IndexFunctions, leftchild)
{
  //counting index of leftChild
  EXPECT_EQ(1, segmentTree::leftchild(0));
  EXPECT_EQ(3, segmentTree::leftchild(1));
  EXPECT_EQ(5, segmentTree::leftchild(2));
  EXPECT_EQ(19, segmentTree::leftchild(9));
}

TEST(IndexFunctions, rightchild)
{
  //counting index of rightChild
  EXPECT_EQ(2, segmentTree::rightchild(0));
  EXPECT_EQ(4, segmentTree::rightchild(1));
  EXPECT_EQ(6, segmentTree::rightchild(2));
  EXPECT_EQ(20, segmentTree::rightchild(9));
}

TEST(IndexFunctions, parent)
{
  //counting index of parent
  EXPECT_EQ(0, segmentTree::parent(1));
  EXPECT_EQ(0, segmentTree::parent(2));
  EXPECT_EQ(1, segmentTree::parent(3));
  EXPECT_EQ(15, segmentTree::parent(31));
  EXPECT_EQ(15, segmentTree::parent(32));
}

//---Testing trivial constructor---
typedef segmentTree::GeneralTree <int, int,
                                  std::less<int>,
                                  std::less<int>,
                                  std::less<int>> IntLessGTree;

TEST (GeneralTree, TrivialConstructor)
{
  IntLessGTree tr;
  TestAccess<IntLessGTree> tr_access(&tr);
  EXPECT_EQ(tr_access.get_data().size(), 0);
  EXPECT_EQ(nullptr, tr_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr_access.get_ptr_to_metaToResultApplier().get());
}


//---Testing copy and move constructors---
struct FakeIntStateFunctor
{
  FakeIntStateFunctor(): _state(0) {}
  explicit FakeIntStateFunctor(int state): _state(state) {}

  int operator()(int a, int b)
  {
    return std::min(a,b);
  }
  int _state;
};

typedef segmentTree::GeneralTree <int, int,
                                  FakeIntStateFunctor,
                                  FakeIntStateFunctor,
                                  FakeIntStateFunctor> FakeFunctorGTree;

TEST (GeneralTree, CopyConstructor)
{
  //Testing copying empty tree
  IntLessGTree tr1;
  TestAccess<IntLessGTree> tr1_access(&tr1);
  
  IntLessGTree tr2(tr1);
  TestAccess<IntLessGTree> tr2_access(&tr2);
  
  //Check
  EXPECT_EQ(0, tr1_access.get_data().size());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_metaToResultApplier().get());

  EXPECT_EQ(0, tr2_access.get_data().size());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_metaToResultApplier().get());


  //Testing copying non-empty tree
  FakeFunctorGTree tr3;
  TestAccess<FakeFunctorGTree> tr3_access(&tr3);
  
  //Initializing
  std::pair<int,int> val(4, 5);
  tr3_access.get_data().push_back(val);
  (tr3_access.get_ptr_to_resultMerger()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(1)));
  (tr3_access.get_ptr_to_metaMerger()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(2)));
  (tr3_access.get_ptr_to_metaToResultApplier()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(3)));

  //Check initialization
  EXPECT_EQ(1, tr3_access.get_data().size());
  EXPECT_EQ(1, (*tr3_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr3_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr3_access.get_ptr_to_metaToResultApplier())._state);

  //Call of copy constructor
  FakeFunctorGTree tr4(tr3);
  TestAccess<FakeFunctorGTree> tr4_access(&tr4);

  //Check
  EXPECT_EQ(1, tr3_access.get_data().size());
  EXPECT_EQ(1, (*tr3_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr3_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr3_access.get_ptr_to_metaToResultApplier())._state);

  EXPECT_EQ(1, tr4_access.get_data().size());
  EXPECT_EQ(1, (*tr4_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr4_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr4_access.get_ptr_to_metaToResultApplier())._state);
}


TEST (GeneralTree, copyOperator)
{
  //Testing copying empty tree
  IntLessGTree tr1;
  TestAccess<IntLessGTree> tr1_access(&tr1);
  
  IntLessGTree tr2 = tr1;
  TestAccess<IntLessGTree> tr2_access(&tr2);
  
  //Check
  EXPECT_EQ(0, tr1_access.get_data().size());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_metaToResultApplier().get());

  EXPECT_EQ(0, tr2_access.get_data().size());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_metaToResultApplier().get());


  //Testing copying non-empty tree
  FakeFunctorGTree tr3;
  TestAccess<FakeFunctorGTree> tr3_access(&tr3);
  
  //Initializing
  std::pair<int,int> val(4, 5);
  tr3_access.get_data().push_back(val);
  (tr3_access.get_ptr_to_resultMerger()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(1)));
  (tr3_access.get_ptr_to_metaMerger()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(2)));
  (tr3_access.get_ptr_to_metaToResultApplier()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(3)));

  //Check initialization
  EXPECT_EQ(1, tr3_access.get_data().size());
  EXPECT_EQ(1, (*tr3_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr3_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr3_access.get_ptr_to_metaToResultApplier())._state);

  //Call of copy assignment operator
  FakeFunctorGTree tr4 = tr3;
  TestAccess<FakeFunctorGTree> tr4_access(&tr4);

  //Check
  EXPECT_EQ(1, tr3_access.get_data().size());
  EXPECT_EQ(1, (*tr3_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr3_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr3_access.get_ptr_to_metaToResultApplier())._state);

  EXPECT_EQ(1, tr4_access.get_data().size());
  EXPECT_EQ(1, (*tr4_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr4_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr4_access.get_ptr_to_metaToResultApplier())._state);
}

TEST(GeneralTree, moveConstructor)
{
  //Testing moving empty tree
  IntLessGTree tr1;
  TestAccess<IntLessGTree> tr1_access(&tr1);
  
  IntLessGTree tr2 (std::move(tr1));
  TestAccess<IntLessGTree> tr2_access(&tr2);
  
  //Check
  EXPECT_EQ(0, tr1_access.get_data().size());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_metaToResultApplier().get());

  EXPECT_EQ(0, tr2_access.get_data().size());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_metaToResultApplier().get());

  //Testing moving non-empty tree
  FakeFunctorGTree tr3;
  TestAccess<FakeFunctorGTree> tr3_access(&tr3);
  
  //Initializing
  std::pair<int,int> val(4, 5);
  tr3_access.get_data().push_back(val);
  (tr3_access.get_ptr_to_resultMerger()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(1)));
  (tr3_access.get_ptr_to_metaMerger()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(2)));
  (tr3_access.get_ptr_to_metaToResultApplier()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(3)));

  //Check initialization
  EXPECT_EQ(1, tr3_access.get_data().size());
  EXPECT_EQ(1, (*tr3_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr3_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr3_access.get_ptr_to_metaToResultApplier())._state);

  //Call of move constructor
  FakeFunctorGTree tr4(std::move(tr3));
  TestAccess<FakeFunctorGTree> tr4_access(&tr4);

  //Check
  EXPECT_EQ(0, tr3_access.get_data().size());
  EXPECT_EQ(nullptr, tr3_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr3_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr3_access.get_ptr_to_metaToResultApplier().get());

  EXPECT_EQ(1, tr4_access.get_data().size());
  EXPECT_EQ(1, (*tr4_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr4_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr4_access.get_ptr_to_metaToResultApplier())._state);
}

TEST(GeneralTree, moveOperator)
{
  //Testing moving empty tree
  IntLessGTree tr1;
  TestAccess<IntLessGTree> tr1_access(&tr1);
  
  IntLessGTree tr2 = std::move(tr1);
  TestAccess<IntLessGTree> tr2_access(&tr2);
  
  //Check
  EXPECT_EQ(0, tr1_access.get_data().size());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr1_access.get_ptr_to_metaToResultApplier().get());

  EXPECT_EQ(0, tr2_access.get_data().size());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr2_access.get_ptr_to_metaToResultApplier().get());

  //Testing moving non-empty tree
  FakeFunctorGTree tr3;
  TestAccess<FakeFunctorGTree> tr3_access(&tr3);
  
  //Initializing
  std::pair<int,int> val(4, 5);
  tr3_access.get_data().push_back(val);
  (tr3_access.get_ptr_to_resultMerger()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(1)));
  (tr3_access.get_ptr_to_metaMerger()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(2)));
  (tr3_access.get_ptr_to_metaToResultApplier()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(3)));

  //Check initialization
  EXPECT_EQ(1, tr3_access.get_data().size());
  EXPECT_EQ(1, (*tr3_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr3_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr3_access.get_ptr_to_metaToResultApplier())._state);

  //Call of move assignment operator
  FakeFunctorGTree tr4 = std::move(tr3);
  TestAccess<FakeFunctorGTree> tr4_access(&tr4);

  //Check
  EXPECT_EQ(0, tr3_access.get_data().size());
  EXPECT_EQ(nullptr, tr3_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr3_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr3_access.get_ptr_to_metaToResultApplier().get());

  EXPECT_EQ(1, tr4_access.get_data().size());
  EXPECT_EQ(1, (*tr4_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr4_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr4_access.get_ptr_to_metaToResultApplier())._state);
}

TEST(GeneralTree, moveOperatorSpeed)
{
  IntLessGTree tr[2];

  TestAccess<IntLessGTree> tr_access[2];
  for (int i = 0; i < 2; ++i)
    tr_access[i]._ptr_to_GTree = &tr[i];

  //Initializing
  (tr_access[0].get_ptr_to_resultMerger()) = std::move(std::unique_ptr
                          <std::less<int>> (new std::less<int>));
  (tr_access[0].get_ptr_to_metaMerger()) = std::move(std::unique_ptr
                          <std::less<int>> (new std::less<int>));
  (tr_access[0].get_ptr_to_metaToResultApplier()) = std::move(std::unique_ptr
                          <std::less<int>> (new std::less<int>));
  for (int i = 0; i < 100500; ++i)
  {
    std::pair<int,int> val(i, i + 1);
    tr_access[0].get_data().push_back(val);
  }

  //moving from one tree to another 100500 times
  for (int i = 0; i < 100500; ++i)
  {
    tr[i % 2] = std::move(tr[(i + 1) % 2]);

    //Adding new values to confuse the compiler and do not allow him to optimize
    std::pair<int,int> val(i, i + 1);
    tr_access[i % 2].get_data().push_back(val);
  }
}

//---Testing build functions---

struct ResultMergerSum
{
  int operator () (int left, int right)
  {
    return left + right;
  }
};

struct EmptyIntMetaMerger
{
  int operator () (int left, int right)
  {
    std::cerr << "calling EmptyIntMetaMerger with params "
              << left << " " << right << std::endl;
    return 0;
  }
};

struct EmptyIntMetaToResultApplier
{
  int operator () (int meta, int res)
  {
    std::cerr << "calling EmptyMetaToResultApplier with params "
              << meta << " " << res << std::endl;
    return res;
  }
};

typedef segmentTree::GeneralTree<int, int,
                                 ResultMergerSum,
                                 EmptyIntMetaMerger,
                                 EmptyIntMetaToResultApplier> DummySumGTree;

TEST(GeneralTree, build)
{
  //In this test we will construct simple sum trees of different sizes
  //and check, that res is well counted and default meta isn't changed.
  //Default meta should be neutral, but it seems to be impossible to check it

  //Segment trees with size = 0 are not allowed

  //constructing tree with size 1
  DummySumGTree tr1;
  tr1.build(1, 1, -5);
  TestAccess<DummySumGTree> access_tr1(&tr1);
  EXPECT_EQ(access_tr1.get_data().size(), 1);
  EXPECT_EQ(access_tr1.get_data()[0].first, 1);
  EXPECT_EQ(access_tr1.get_data()[0].second, -5);

  //constructing tree with size 2
  DummySumGTree tr2;
  tr2.build(2, 1, -5);
  TestAccess<DummySumGTree> access_tr2(&tr2);
  EXPECT_EQ(access_tr2.get_data().size(), 3);
  EXPECT_EQ(access_tr2.get_data()[0].first, 2);
  EXPECT_EQ(access_tr2.get_data()[0].second, -5);
  EXPECT_EQ(access_tr2.get_data()[1].first, 1);
  EXPECT_EQ(access_tr2.get_data()[1].second, -5);
  EXPECT_EQ(access_tr2.get_data()[2].first, 1);
  EXPECT_EQ(access_tr2.get_data()[2].second, -5);

  //constructing tree with size 3
  DummySumGTree tr3;
  tr3.build(3, 1, -5);
  TestAccess<DummySumGTree> access_tr3(&tr3);
  EXPECT_EQ(access_tr3.get_data().size(), 5);
  EXPECT_EQ(access_tr3.get_data()[0].first, 3);
  EXPECT_EQ(access_tr3.get_data()[0].second, -5);
  EXPECT_EQ(access_tr3.get_data()[1].first, 2);
  EXPECT_EQ(access_tr3.get_data()[1].second, -5);
  EXPECT_EQ(access_tr3.get_data()[2].first, 1);
  EXPECT_EQ(access_tr3.get_data()[2].second, -5);
  EXPECT_EQ(access_tr3.get_data()[3].first, 1);
  EXPECT_EQ(access_tr3.get_data()[3].second, -5);
  EXPECT_EQ(access_tr3.get_data()[4].first, 1);
  EXPECT_EQ(access_tr3.get_data()[4].second, -5);
}

TEST(GeneralTree, build_from_range)
{
  int vals[3] = {1, 2, 3};
  
  DummySumGTree tr1;
  tr1.build_from_range(vals, vals + 1, -5);

  TestAccess<DummySumGTree> access_tr1(&tr1);
  EXPECT_EQ(access_tr1.get_data().size(), 1);
  EXPECT_EQ(access_tr1.get_data()[0].first, 1);
  EXPECT_EQ(access_tr1.get_data()[0].second, -5);

  DummySumGTree tr2;
  tr2.build_from_range(vals, vals + 2, -5);
  TestAccess<DummySumGTree> access_tr2(&tr2);
  EXPECT_EQ(access_tr2.get_data().size(), 3);
  EXPECT_EQ(access_tr2.get_data()[0].first, 3);
  EXPECT_EQ(access_tr2.get_data()[0].second, -5);
  EXPECT_EQ(access_tr2.get_data()[1].first, 1);
  EXPECT_EQ(access_tr2.get_data()[1].second, -5);
  EXPECT_EQ(access_tr2.get_data()[2].first, 2);
  EXPECT_EQ(access_tr2.get_data()[2].second, -5);

  //constructing tree with size 3
  DummySumGTree tr3;
  tr3.build_from_range(vals, vals + 3, -5);
  TestAccess<DummySumGTree> access_tr3(&tr3);
  EXPECT_EQ(access_tr3.get_data().size(), 5);
  EXPECT_EQ(access_tr3.get_data()[0].first, 6);
  EXPECT_EQ(access_tr3.get_data()[0].second, -5);
  EXPECT_EQ(access_tr3.get_data()[1].first, 5);
  EXPECT_EQ(access_tr3.get_data()[1].second, -5);
  EXPECT_EQ(access_tr3.get_data()[2].first, 1);
  EXPECT_EQ(access_tr3.get_data()[2].second, -5);
  EXPECT_EQ(access_tr3.get_data()[3].first, 2);
  EXPECT_EQ(access_tr3.get_data()[3].second, -5);
  EXPECT_EQ(access_tr3.get_data()[4].first, 3);
  EXPECT_EQ(access_tr3.get_data()[4].second, -5);
}

TEST(GeneralTree, reset)
{
  DummySumGTree tr;
  tr.build(3);
  tr.reset();
  TestAccess<DummySumGTree> tr_access(&tr);
  EXPECT_EQ(tr_access.get_data().size(), 0);
  EXPECT_EQ(nullptr, tr_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr_access.get_ptr_to_metaToResultApplier().get());
}

struct Borders
{
  Borders():_l(-1), _r(-1) {}
  Borders(size_t l, size_t r): _l(l), _r(r) {}
  
  size_t _l;
  size_t _r;
};

struct ResultMergerBorders
{
  Borders operator () (Borders left, Borders right)
  {
    return Borders(left._l, right._r);
  }
};

struct EmptyBordersMetaToResultApplier
{
  Borders operator () (int meta, Borders res)
  {
    std::cerr << "calling EmptyMetaToResultApplier with params "
              << meta << " " << res._l << " " << res._r << std::endl;
    return res;
  }
};

typedef segmentTree::GeneralTree <Borders, int,
                                  ResultMergerBorders,
                                  EmptyIntMetaMerger,
                                  EmptyBordersMetaToResultApplier> BordersGTree;
