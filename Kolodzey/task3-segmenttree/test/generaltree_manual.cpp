#include <string>
#include <random>
#include "gtest/gtest.h"
#include "generaltree.h"
#include "access.h"
#include "specialization.h"

//---Testing index functions---
TEST(IndexFunctions, sizeOfTree)
{
  EXPECT_EQ( 1, segmentTree::sizeOfTree(1));
  EXPECT_EQ( 3, segmentTree::sizeOfTree(2));
  EXPECT_EQ( 7, segmentTree::sizeOfTree(3));
  EXPECT_EQ( 7, segmentTree::sizeOfTree(4));
  EXPECT_EQ(15, segmentTree::sizeOfTree(5));
  EXPECT_EQ(15, segmentTree::sizeOfTree(6));
  EXPECT_EQ(15, segmentTree::sizeOfTree(7));
  EXPECT_EQ(15, segmentTree::sizeOfTree(8));
  EXPECT_EQ(31, segmentTree::sizeOfTree(9));
  EXPECT_EQ(31, segmentTree::sizeOfTree(16));
  EXPECT_EQ(63, segmentTree::sizeOfTree(17));
}


TEST(IndexFunctions, lastMetaVertexIndex)
{
  //counting lastMetaVertexIndex by treesize
  EXPECT_EQ((size_t)(-1), segmentTree::lastMetaVertexIndex(1));
  EXPECT_EQ(0, segmentTree::lastMetaVertexIndex(3));
  EXPECT_EQ(2, segmentTree::lastMetaVertexIndex(7));
  EXPECT_EQ(14, segmentTree::lastMetaVertexIndex(31));
}


TEST(IndexFunctions, leftchildInd)
{
  //counting index of leftChild
  EXPECT_EQ(1, segmentTree::leftchildInd(0));
  EXPECT_EQ(3, segmentTree::leftchildInd(1));
  EXPECT_EQ(5, segmentTree::leftchildInd(2));
  EXPECT_EQ(19, segmentTree::leftchildInd(9));
}

TEST(IndexFunctions, rightchild)
{
  //counting index of rightChild
  EXPECT_EQ(2, segmentTree::rightchildInd(0));
  EXPECT_EQ(4, segmentTree::rightchildInd(1));
  EXPECT_EQ(6, segmentTree::rightchildInd(2));
  EXPECT_EQ(20, segmentTree::rightchildInd(9));
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

TEST(IndexFunctions, nMetaVertexes)
{
  EXPECT_EQ( 0, segmentTree::nMetaVertexes(1));
  EXPECT_EQ( 1, segmentTree::nMetaVertexes(2));
  EXPECT_EQ( 3, segmentTree::nMetaVertexes(3));
  EXPECT_EQ( 3, segmentTree::nMetaVertexes(4));
  EXPECT_EQ( 7, segmentTree::nMetaVertexes(5));
  EXPECT_EQ( 7, segmentTree::nMetaVertexes(6));
  EXPECT_EQ( 7, segmentTree::nMetaVertexes(7));
  EXPECT_EQ( 7, segmentTree::nMetaVertexes(8));
  EXPECT_EQ(15, segmentTree::nMetaVertexes(9));
  EXPECT_EQ(15, segmentTree::nMetaVertexes(16));
  EXPECT_EQ(31, segmentTree::nMetaVertexes(17));
}

TEST(IndexFunctions, lengthOfLowerLevel)
{
  EXPECT_EQ(1, segmentTree::lengthOfLowerLevel(1));
  EXPECT_EQ(2, segmentTree::lengthOfLowerLevel(3));
  EXPECT_EQ(4, segmentTree::lengthOfLowerLevel(7));
  EXPECT_EQ(8, segmentTree::lengthOfLowerLevel(15));
}

TEST(IndexFunctions, countLeftR)
{
  EXPECT_EQ(0, segmentTree::countLeftSonR(0, 1));
  EXPECT_EQ(1, segmentTree::countLeftSonR(0, 3));
  EXPECT_EQ(5, segmentTree::countLeftSonR(4, 7));
  EXPECT_EQ(2, segmentTree::countLeftSonR(2, 3));
}

TEST(IndexFunctions, countRightL)
{
  EXPECT_EQ(1, segmentTree::countRightSonL(0, 1));
  EXPECT_EQ(2, segmentTree::countRightSonL(0, 3));
  EXPECT_EQ(6, segmentTree::countRightSonL(4, 7));
  EXPECT_EQ(3, segmentTree::countRightSonL(2, 3));
}

//---Testing trivial constructor---
typedef segmentTree::GeneralTree <int, int,
                                  std::less<int>,
                                  std::less<int>,
                                  std::less<int>> IntLessGTree;

template <class T> void checkIfTreeIsEmpty (T& tree)
{
  TestAccess<T> access_tree(&tree);
  EXPECT_EQ(0, access_tree.get_data().size());
  EXPECT_EQ(nullptr, access_tree.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, access_tree.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, access_tree.get_ptr_to_metaToResultApplier().get());
  EXPECT_EQ(0, access_tree.get_length());
}

TEST (GeneralTree, TrivialConstructor)
{
  IntLessGTree tr;
  checkIfTreeIsEmpty(tr);
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

void sampleInit1FFTree(FakeFunctorGTree &tr)
{
  TestAccess<FakeFunctorGTree> tr_access(&tr);
  std::pair<int,std::unique_ptr<int>> val(4, std::unique_ptr<int>(new int(5)));
  tr_access.get_data().push_back(std::move(val));

  (tr_access.get_ptr_to_resultMerger()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(1)));
  (tr_access.get_ptr_to_metaMerger()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(2)));
  (tr_access.get_ptr_to_metaToResultApplier()) = std::move(std::unique_ptr
                        <FakeIntStateFunctor> (new FakeIntStateFunctor(3)));
  tr_access.get_length() = 4;
}

void checkEqToSampleInit1FFTree(FakeFunctorGTree &tr)
{
  TestAccess<FakeFunctorGTree> tr_access(&tr);
  EXPECT_EQ(1, tr_access.get_data().size());
  EXPECT_EQ(4, tr_access.get_length());
  EXPECT_EQ(1, (*tr_access.get_ptr_to_resultMerger())._state);
  EXPECT_EQ(2, (*tr_access.get_ptr_to_metaMerger())._state);
  EXPECT_EQ(3, (*tr_access.get_ptr_to_metaToResultApplier())._state);
}

TEST (GeneralTree, CopyConstructor)
{
  //Testing copying empty tree
  IntLessGTree tr1;  
  IntLessGTree tr2(tr1);

  checkIfTreeIsEmpty(tr1);
  checkIfTreeIsEmpty(tr2);

  //Testing copying non-empty tree
  FakeFunctorGTree tr3;
  sampleInit1FFTree(tr3);
  checkEqToSampleInit1FFTree(tr3);

  FakeFunctorGTree tr4(tr3);
  checkEqToSampleInit1FFTree(tr3);
  checkEqToSampleInit1FFTree(tr4);
}

TEST (GeneralTree, copyAssignement)
{
  //Testing copying empty tree
  IntLessGTree tr1;
  IntLessGTree tr2;
  tr2 = tr1;

  checkIfTreeIsEmpty(tr1);
  checkIfTreeIsEmpty(tr2);

  //Testing copying non-empty tree
  FakeFunctorGTree tr3;
  sampleInit1FFTree(tr3);
  checkEqToSampleInit1FFTree(tr3);

  FakeFunctorGTree tr4;
  tr4 = tr3;
  checkEqToSampleInit1FFTree(tr3);
  checkEqToSampleInit1FFTree(tr4);
}



TEST (GeneralTree, moveConstructor)
{
  //Testing copying empty tree
  IntLessGTree tr1;
  IntLessGTree tr2(std::move(tr1));

  checkIfTreeIsEmpty(tr1);
  checkIfTreeIsEmpty(tr2);

  //Testing copying non-empty tree
  FakeFunctorGTree tr3;
  sampleInit1FFTree(tr3);
  checkEqToSampleInit1FFTree(tr3);

  FakeFunctorGTree tr4(std::move(tr3));
  checkIfTreeIsEmpty(tr3);
  checkEqToSampleInit1FFTree(tr4);
}



TEST (GeneralTree, moveAssignement)
{
  //Testing copying empty tree
  IntLessGTree tr1;
  IntLessGTree tr2 = std::move(tr1);

  checkIfTreeIsEmpty(tr1);
  checkIfTreeIsEmpty(tr2);

  //Testing copying non-empty tree
  FakeFunctorGTree tr3;
  sampleInit1FFTree(tr3);
  checkEqToSampleInit1FFTree(tr3);

  FakeFunctorGTree tr4 = std::move(tr3);
  checkIfTreeIsEmpty(tr3);
  checkEqToSampleInit1FFTree(tr4);
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
  tr_access[0].get_length() = 100500;

  for (int i = 0; i < 100500; ++i)
  {
    std::pair<int,std::unique_ptr<int>> val(i,
                  std::unique_ptr<int>(new int(i + 1)));
    tr_access[0].get_data().push_back(std::move(val));
  }

  //moving from one tree to another 100500 times
  for (int i = 1; i < 100500; ++i)
  {
    tr[i % 2] = std::move(tr[(i + 1) % 2]);

    //Adding new values to confuse the compiler and do not allow him to optimize
    std::pair<int,std::unique_ptr<int>> val(i,
                  std::unique_ptr<int>(new int(i + 1)));    
    tr_access[i % 2].get_data().push_back(std::move(val));
  }
}

//---Testing build functions---

//definition of DummySum tree
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

  //Segment trees with size = 0 are not allowed

  //constructing tree with size 1
  DummySumGTree tr1;
  tr1.build(1, 1);
  
  TestAccess<DummySumGTree> access_tr1(&tr1);
  EXPECT_EQ(1, tr1.length());
  EXPECT_EQ(access_tr1.get_data().size(), 1);
  EXPECT_EQ(access_tr1.get_data()[0].first, 1);
  EXPECT_EQ(access_tr1.get_data()[0].second, nullptr);

  //constructing tree with size 2
  DummySumGTree tr2;
  tr2.build(2, 1);
  TestAccess<DummySumGTree> access_tr2(&tr2);
  EXPECT_EQ(2, tr2.length());
  EXPECT_EQ(access_tr2.get_data().size(), 3);
  EXPECT_EQ(access_tr2.get_data()[0].first, 2);
  EXPECT_EQ(access_tr2.get_data()[0].second, nullptr);
  EXPECT_EQ(access_tr2.get_data()[1].first, 1);
  EXPECT_EQ(access_tr2.get_data()[1].second, nullptr);
  EXPECT_EQ(access_tr2.get_data()[2].first, 1);
  EXPECT_EQ(access_tr2.get_data()[2].second, nullptr);

  //constructing tree with size 3
  DummySumGTree tr3;
  tr3.build(3, 1);
  TestAccess<DummySumGTree> access_tr3(&tr3);
  EXPECT_EQ(3, tr3.length());
  EXPECT_EQ(access_tr3.get_data().size(), 7);
  EXPECT_EQ(access_tr3.get_data()[0].first, 4);
  EXPECT_EQ(access_tr3.get_data()[0].second, nullptr);
  EXPECT_EQ(access_tr3.get_data()[1].first, 2);
  EXPECT_EQ(access_tr3.get_data()[1].second, nullptr);
  EXPECT_EQ(access_tr3.get_data()[2].first, 2);
  EXPECT_EQ(access_tr3.get_data()[2].second, nullptr);
  EXPECT_EQ(access_tr3.get_data()[3].first, 1);
  EXPECT_EQ(access_tr3.get_data()[3].second, nullptr);
  EXPECT_EQ(access_tr3.get_data()[4].first, 1);
  EXPECT_EQ(access_tr3.get_data()[4].second, nullptr);
  EXPECT_EQ(access_tr3.get_data()[5].first, 1);
  EXPECT_EQ(access_tr3.get_data()[5].second, nullptr);
  EXPECT_EQ(access_tr3.get_data()[6].first, 1);
  EXPECT_EQ(access_tr3.get_data()[6].second, nullptr);

  //and size 4
  DummySumGTree tr;
  tr.build(4, 1);
  TestAccess<DummySumGTree> access_tr(&tr);
  EXPECT_EQ(access_tr.get_data().size(), 7);
  EXPECT_EQ(access_tr.get_data()[0].first, 4);
  EXPECT_EQ(access_tr.get_data()[0].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[1].first, 2);
  EXPECT_EQ(access_tr.get_data()[1].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[2].first, 2);
  EXPECT_EQ(access_tr.get_data()[2].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[3].first, 1);
  EXPECT_EQ(access_tr.get_data()[3].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[4].first, 1);
  EXPECT_EQ(access_tr.get_data()[4].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[5].first, 1);
  EXPECT_EQ(access_tr.get_data()[5].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[6].first, 1);
  EXPECT_EQ(access_tr.get_data()[6].second, nullptr);

  tr.reset();
  tr.build(100500, 1); //just to test speed
}


TEST(GeneralTree, build_from_range)
{
  int vals[100500];
  for (int i = 0; i < 100500; ++i)
    vals[i] = i + 1;
  
  DummySumGTree tr1;
  tr1.build_from_range(vals, vals + 1);
  
  TestAccess<DummySumGTree> access_tr1(&tr1);
  EXPECT_EQ(access_tr1.get_data().size(), 1);
  EXPECT_EQ(access_tr1.get_data()[0].first, 1);
  EXPECT_EQ(access_tr1.get_data()[0].second, nullptr);

  DummySumGTree tr2;
  tr2.build_from_range(vals, vals + 2);
  TestAccess<DummySumGTree> access_tr2(&tr2);
  EXPECT_EQ(access_tr2.get_data().size(), 3);
  EXPECT_EQ(access_tr2.get_data()[0].first, 3);
  EXPECT_EQ(access_tr2.get_data()[0].second, nullptr);
  EXPECT_EQ(access_tr2.get_data()[1].first, 1);
  EXPECT_EQ(access_tr2.get_data()[1].second, nullptr);
  EXPECT_EQ(access_tr2.get_data()[2].first, 2);
  EXPECT_EQ(access_tr2.get_data()[2].second, nullptr);

  //constructing tree with size 3
  DummySumGTree tr3;
  tr3.build_from_range(vals, vals + 3);
  TestAccess<DummySumGTree> access_tr3(&tr3);
  EXPECT_EQ(access_tr3.get_data().size(), 7);
  EXPECT_EQ(access_tr3.get_data()[0].first, 6);
  EXPECT_EQ(access_tr3.get_data()[0].second, nullptr);

  EXPECT_EQ(access_tr3.get_data()[1].first, 3);
  EXPECT_EQ(access_tr3.get_data()[1].second, nullptr);
  EXPECT_EQ(access_tr3.get_data()[2].first, 3);
  EXPECT_EQ(access_tr3.get_data()[2].second, nullptr);

  EXPECT_EQ(access_tr3.get_data()[3].first, 1);
  EXPECT_EQ(access_tr3.get_data()[3].second, nullptr);
  EXPECT_EQ(access_tr3.get_data()[4].first, 2);
  EXPECT_EQ(access_tr3.get_data()[4].second, nullptr);
  EXPECT_EQ(access_tr3.get_data()[5].first, 3);
  EXPECT_EQ(access_tr3.get_data()[5].second, nullptr);
  EXPECT_EQ(access_tr3.get_data()[6].first, 0);
  EXPECT_EQ(access_tr3.get_data()[6].second, nullptr);

  tr3.reset();
  tr3.build_from_range(vals, vals + 100500); //just to test speed
}


//---reset---
TEST(GeneralTree, reset)
{
  DummySumGTree tr;
  TestAccess<DummySumGTree> tr_access(&tr);

  //initializing tree with some data
  for (int i = 0; i < 15; ++i)
  {
      std::pair<int,std::unique_ptr<int>> val(i,
                    std::unique_ptr<int>(new int(i + 1)));
      tr_access.get_data().push_back(std::move(val));
  }

  tr_access.get_length() = 6;

  tr_access.get_ptr_to_resultMerger() = std::move(std::unique_ptr
                      <ResultMergerSum> (new ResultMergerSum()));

  tr_access.get_ptr_to_metaMerger() = std::move(std::unique_ptr
                <EmptyIntMetaMerger> (new EmptyIntMetaMerger()));

  tr_access.get_ptr_to_metaToResultApplier() = std::move(std::unique_ptr
                 <EmptyIntMetaToResultApplier> (new EmptyIntMetaToResultApplier()));

  //resetting and checking
  tr.reset();  
  checkIfTreeIsEmpty(tr);
}

//---get---
TEST (GeneralTree, getForSizeUpTo4)
{
    //All the requests get on tree of size of <=4
    for (int size = 1; size <= 4; ++size)
    {
      DummySumGTree tr;
      tr.build(size, 1);
      std::cerr << size << std::endl;
      for (int i = 0; i < size; ++i)
        for (int j = i; j < size; ++j)
        {
          EXPECT_EQ(tr.get(i, j), j - i + 1);
        }
    }
}

TEST(GeneralTree, modify_nonmodified_tree_with_size_up_to_4)
{
  DummySumGTree tr;
  TestAccess<DummySumGTree> access_tr(&tr);

  //length = 1
  std::cerr << "length = 1, modify(0, 0, 1)" << std::endl;
  tr.build(1, 1);
  tr.modify(0, 0, 1);
  EXPECT_EQ(*(access_tr.get_data()[0].second), 1);


  std::cerr << "length = 2, modify(0, 1, 1)" << std::endl;
  tr.reset();
  tr.build(2, 1);
  tr.modify(0, 1, 1);

  EXPECT_EQ(*(access_tr.get_data()[0].second), 1);

  EXPECT_EQ(access_tr.get_data()[1].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[2].second, nullptr);


  std::cerr << "length = 2, modify(0, 1, 1)" << std::endl;
  tr.reset();
  tr.build(2, 1);
  tr.modify(0, 0, 1);

  EXPECT_EQ(access_tr.get_data()[0].second, nullptr);

  EXPECT_EQ(*(access_tr.get_data()[1].second), 1);
  EXPECT_EQ(access_tr.get_data()[2].second, nullptr);


  std::cerr << "length = 2, modify(1, 1, 1)" << std::endl;
  tr.reset();
  tr.build(2, 1);
  tr.modify(1, 1, 1);
  EXPECT_EQ(access_tr.get_data()[0].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[1].second, nullptr);
  EXPECT_EQ(*(access_tr.get_data()[2].second), 1);


  std::cerr << "length = 3, modify(0, 2, 1)" << std::endl;
  tr.reset();
  tr.build(3, 1);
  tr.modify(0, 2, 1);
  EXPECT_EQ(access_tr.get_data()[0].second, nullptr);

  EXPECT_EQ(*(access_tr.get_data()[1].second), 1);
  EXPECT_EQ(access_tr.get_data()[2].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[3].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[4].second, nullptr);
  EXPECT_EQ(*(access_tr.get_data()[5].second), 1);
  EXPECT_EQ(access_tr.get_data()[6].second, nullptr);


  std::cerr << "length = 3, modify(0, 1, 1)" << std::endl;
  tr.reset();
  tr.build(3, 1);
  tr.modify(0, 1, 1);
  EXPECT_EQ(access_tr.get_data()[0].second, nullptr);

  EXPECT_EQ(*(access_tr.get_data()[1].second), 1);
  EXPECT_EQ(access_tr.get_data()[2].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[3].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[4].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[5].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[6].second, nullptr);


  std::cerr << "length = 3, modify(1, 2, 1)" << std::endl;
  tr.reset();
  tr.build(3, 1);
  tr.modify(1, 2, 1);
  EXPECT_EQ(access_tr.get_data()[0].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[1].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[2].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[3].second, nullptr);
  EXPECT_EQ(*(access_tr.get_data()[4].second), 1);
  EXPECT_EQ(*(access_tr.get_data()[5].second), 1);
  EXPECT_EQ(access_tr.get_data()[6].second, nullptr);

  std::cerr << "length = 3, modify(0, 0, 1)" << std::endl;
  tr.reset();
  tr.build(3, 1);
  tr.modify(0, 0, 1);
  EXPECT_EQ(access_tr.get_data()[0].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[1].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[2].second, nullptr);

  EXPECT_EQ(*(access_tr.get_data()[3].second), 1);
  EXPECT_EQ(access_tr.get_data()[4].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[5].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[6].second, nullptr);

  std::cerr << "length = 3, modify(1, 1, 1)" << std::endl;
  tr.reset();
  tr.build(3, 1);
  tr.modify(1, 1, 1);
  EXPECT_EQ(access_tr.get_data()[0].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[1].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[2].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[3].second, nullptr);
  EXPECT_EQ(*(access_tr.get_data()[4].second), 1);
  EXPECT_EQ(access_tr.get_data()[5].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[6].second, nullptr);

  std::cerr << "length = 3, modify(2, 2, 1)" << std::endl;
  tr.reset();
  tr.build(3, 1);
  tr.modify(2, 2, 1);
  EXPECT_EQ(access_tr.get_data()[0].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[1].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[2].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[3].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[4].second, nullptr);
  EXPECT_EQ(*(access_tr.get_data()[5].second), 1);
  EXPECT_EQ(access_tr.get_data()[6].second, nullptr);
}

//segmentTree::llsum_add is defined in specialization.h

TEST (GeneralTree, get_modify_sum_add1)
{
  segmentTree::llsum_add tr;
  std::vector<segmentTree::ResultType_llsum> v;
  for (int i = 0; i < 3; ++i)
    v.push_back(segmentTree::ResultType_llsum(i, i, 0));
  tr.build_from_range(v.begin(), v.end());
  TestAccess<segmentTree::llsum_add> access_tr(&tr);
  EXPECT_EQ(access_tr.get_data().size(), 7);
  
  EXPECT_EQ(access_tr.get_data()[0].second, nullptr);   //res[0] undefined
  
  //--
  EXPECT_EQ(access_tr.get_data()[1].first._sum, 0);
  EXPECT_EQ(access_tr.get_data()[1].first._l, 0);
  EXPECT_EQ(access_tr.get_data()[1].first._r, 1);
  EXPECT_EQ(access_tr.get_data()[1].second, nullptr);
  
  EXPECT_EQ(access_tr.get_data()[2].second, nullptr);   //res[2] undefined
  
  //--
  EXPECT_EQ(access_tr.get_data()[3].first._sum, 0);
  EXPECT_EQ(access_tr.get_data()[3].first._l, 0);
  EXPECT_EQ(access_tr.get_data()[3].first._r, 0);
  EXPECT_EQ(access_tr.get_data()[3].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[4].first._sum, 0);
  EXPECT_EQ(access_tr.get_data()[4].first._l, 1);
  EXPECT_EQ(access_tr.get_data()[4].first._r, 1);
  EXPECT_EQ(access_tr.get_data()[4].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[5].first._sum, 0);
  EXPECT_EQ(access_tr.get_data()[5].first._l, 2);
  EXPECT_EQ(access_tr.get_data()[5].first._r, 2);
  EXPECT_EQ(access_tr.get_data()[5].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[6].second, nullptr);   //res[6] undefined

  for (int i = 0; i < 3; ++i)
    for (int j = i; j < 3; ++j)
    {
      EXPECT_EQ(0, tr.get(i,j)._sum);
      EXPECT_EQ(i, tr.get(i,j)._l);
      EXPECT_EQ(j, tr.get(i,j)._r);
    }

  tr.modify(0, 2, 1);
  for (int i = 0; i < 3; ++i)
    for (int j = i; j < 3; ++j)
    {
      EXPECT_EQ(j - i + 1, tr.get(i,j)._sum);
      EXPECT_EQ(i, tr.get(i,j)._l);
      EXPECT_EQ(j, tr.get(i,j)._r);
    }

  tr.modify(0, 2, -1);
  for (int i = 0; i < 3; ++i)
    for (int j = i; j < 3; ++j)
    {
      EXPECT_EQ(0, tr.get(i,j)._sum);
      EXPECT_EQ(i, tr.get(i,j)._l);
      EXPECT_EQ(j, tr.get(i,j)._r);
    }
}

TEST (GeneralTree, get_modify_sum_add2)
{
  segmentTree::llsum_add tr;
  std::vector<segmentTree::ResultType_llsum> v;
  for (int i = 0; i < 10; ++i)
    v.push_back(segmentTree::ResultType_llsum(i, i, 0));

  tr.build_from_range(v.begin(), v.end());
  tr.modify(0, 8, 10);
  TestAccess<segmentTree::llsum_add> access_tr(&tr);
  EXPECT_EQ(*(access_tr.get_data()[1].second), 10);
  EXPECT_EQ(*(access_tr.get_data()[23].second), 10);

  tr.modify(1, 4, -3);
  EXPECT_EQ(access_tr.get_data()[1].first._sum, 68);
  EXPECT_EQ(access_tr.get_data()[1].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[3].first._sum, 31);
  EXPECT_EQ(access_tr.get_data()[3].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[4].first._sum, 37);
  EXPECT_EQ(access_tr.get_data()[4].second, nullptr);

  EXPECT_EQ(access_tr.get_data()[7].first._sum, 17);
  EXPECT_EQ(access_tr.get_data()[7].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[8].first._sum, 0);
  EXPECT_EQ(*(access_tr.get_data()[8].second), 7);
  EXPECT_EQ(access_tr.get_data()[9].first._sum, 17);
  EXPECT_EQ(access_tr.get_data()[9].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[10].first._sum, 0);
  EXPECT_EQ(*(access_tr.get_data()[10].second), 10);

  EXPECT_EQ(access_tr.get_data()[15].first._sum, 0);
  EXPECT_EQ(*(access_tr.get_data()[15].second), 10);
  EXPECT_EQ(access_tr.get_data()[16].first._sum, 0);
  EXPECT_EQ(*(access_tr.get_data()[16].second), 7);
  EXPECT_EQ(access_tr.get_data()[17].first._sum, 0);
  EXPECT_EQ(access_tr.get_data()[17].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[18].first._sum, 0);
  EXPECT_EQ(access_tr.get_data()[18].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[19].first._sum, 0);
  EXPECT_EQ(*(access_tr.get_data()[19].second), 7);
  EXPECT_EQ(access_tr.get_data()[20].first._sum, 0);
  EXPECT_EQ(*(access_tr.get_data()[20].second), 10);
  EXPECT_EQ(access_tr.get_data()[21].first._sum, 0);
  EXPECT_EQ(access_tr.get_data()[21].second, nullptr);
  EXPECT_EQ(access_tr.get_data()[22].first._sum, 0);
  EXPECT_EQ(access_tr.get_data()[22].second, nullptr);

  EXPECT_EQ(37, tr.get(4, 7)._sum);
}

//---speed tests for get and modify---
void callALotOfModify(segmentTree::llsum_add& tr, int nReq)
{
  std::default_random_engine generator;
  std::uniform_int_distribution<int> indDistribution(0, tr.length() / 2 - 5);
  std::uniform_int_distribution<int> valDistribution(-100500100, 500100500);
  for (int i = 1; i < nReq; ++i)
  {

    if (i % 500 == 0)
      std::cerr << "n = " << i << "; mod = " << segmentTree::N_INNER_MODIFY
                << "; get = " << segmentTree::N_INNER_GET << std::endl;
    
    int l = indDistribution(generator);
    int r = l + indDistribution(generator);
    int x = valDistribution(generator);
    tr.modify(l, r, x);
  }
}

void callALotOfGet(segmentTree::llsum_add& tr, int nReq)
{
  std::default_random_engine generator;
  std::uniform_int_distribution<int> indDistribution(0, tr.length() / 2 - 5);
  for (int i = 1; i < nReq; ++i)
  {

    if (i % 500 == 0)
      std::cerr << "n = " << i << "; mod = " << segmentTree::N_INNER_MODIFY
                << "; get = " << segmentTree::N_INNER_GET << std::endl;
    
    int l = indDistribution(generator);
    int r = l + indDistribution(generator);
    tr.get(l, r);
  }
}

TEST(GeneralTree, numberOfRequests_modify)
{
  segmentTree::llsum_add tr;
  std::vector<segmentTree::ResultType_llsum> v;
  v.resize(100500);
  for (int i = 0; i <= 100500; ++i)
    v[i] = segmentTree::ResultType_llsum(i, i, 0);
  std::cerr << "initialized vector of values" << std::endl;
  tr.build_from_range(v.begin(),v.end());
  std::cerr << "initialized tree" << std::endl;
  callALotOfModify(tr, 100500);
}

TEST(GeneralTree, numberOfRequests_get)
{
  segmentTree::llsum_add tr;
  std::vector<segmentTree::ResultType_llsum> v;
  for (int i = 0; i <= 100500; ++i)
    v.push_back(segmentTree::ResultType_llsum(i, i, 0));
  std::cerr << "initialized vector of values" << std::endl;
  tr.build_from_range(v.begin(),v.end());
  std::cerr << "initialized tree" << std::endl;
  callALotOfModify(tr, 100500);

  segmentTree::N_INNER_GET = 0;
  segmentTree::N_INNER_MODIFY = 0;
  callALotOfGet(tr, 100500);
}
