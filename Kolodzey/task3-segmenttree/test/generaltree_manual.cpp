#include <string>
#include "gtest/gtest.h"
#include "generaltree.h"
#include "access.h"

typedef segmentTree::GeneralTree <int, int,
                                  std::less<int>,
                                  std::less<int>,
                                  std::less<int>> IntLessGTree;

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

TEST (GeneralTree, TrivialConstructor)
{
  IntLessGTree tr;
  TestAccess<IntLessGTree> tr_access(&tr);
  EXPECT_EQ(tr_access.get_data().size(), 0);
  EXPECT_EQ(nullptr, tr_access.get_ptr_to_resultMerger().get());
  EXPECT_EQ(nullptr, tr_access.get_ptr_to_metaMerger().get());
  EXPECT_EQ(nullptr, tr_access.get_ptr_to_metaToResultApplier().get());
}


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