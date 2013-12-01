#include <fstream>
#include "gtest/gtest.h"
#include "biheap.h"
#include "dummyheap.h"
#include "access.h"

TEST(informatics755, test5withCheckingWhatValuesAreHold)
{
  //std::cerr << "starting" << std::endl;

  std::ifstream in("informatics755/05.in");
  std::ifstream ans("informatics755/05.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  DummyHeap<int,std::greater<int> > stupid((std::greater<int>()));
  TestAccess <BiHeap<int,std::greater<int> >, int> heap_access;
  heap_access._biheap = &heap;
  std::vector<int> vals;
  int N;
  in >> N;
  int sz = 0;
  for (int i = 0; i < N; ++i)
  {
  	int cmd;
  	in >> cmd;
  	if (cmd == 0)
  	{
      ++sz;
  		int val;
  		in >> val;
  		heap.insert(val);
      stupid.insert(val);
      EXPECT_EQ(stupid.top(), heap.top());
      //std::cerr << "inserted" << val << std::endl;
  	}
  	else
  	{
      --sz;
  		int val;
  		ans >> val;
  		EXPECT_EQ(val, heap.top());
  		//std::cerr << "looked at top" << std::endl;
  		EXPECT_EQ(heap.pop(), 1);
  		//std::cerr << "deleted top" << std::endl;
      stupid.pop_top();
  	}
    vals.clear();
    TestAccess<BiHeap<int,std::greater<int> >,int>::get_all_val(heap,vals);
    EXPECT_EQ(vals.size(),sz);
    sort(vals.begin(), vals.end(), std::greater<int>());
    for (int j = 0; j < sz; ++j)
      EXPECT_EQ(vals[j],stupid._pv[j]);
    //std::cerr << "passed " << i << std::endl;
    //std::cerr << "now sz " << sz << std::endl;
    //std::cerr << "now elem";
    //for (int j = 0; j < sz; ++j)
    //	std::cerr << " " << vals[j];
    //std::cerr << std::endl;
    //BiHeap<int,std::greater<int> >::Itlist it;
    //for (it = heap_access._forest().begin(); it != heap_access._forest().end(); ++it)
    //	std::cerr << (*it)->get_level() << " " << (*it)->get_val() << std::endl;
    //std::cerr << std::endl << std::endl;
  }
  in.close();
  ans.close();
}

TEST(informatics755, test1)
{
  std::ifstream in("informatics755/01.in");
  std::ifstream ans("informatics755/01.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test2)
{
  std::ifstream in("informatics755/02.in");
  std::ifstream ans("informatics755/02.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test3)
{
  std::ifstream in("informatics755/03.in");
  std::ifstream ans("informatics755/03.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test4)
{
  std::ifstream in("informatics755/04.in");
  std::ifstream ans("informatics755/04.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test6)
{
  std::ifstream in("informatics755/06.in");
  std::ifstream ans("informatics755/06.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test7)
{
  std::ifstream in("informatics755/07.in");
  std::ifstream ans("informatics755/07.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test8)
{
  std::ifstream in("informatics755/08.in");
  std::ifstream ans("informatics755/08.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test9)
{
  std::ifstream in("informatics755/09.in");
  std::ifstream ans("informatics755/09.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test10)
{
  std::ifstream in("informatics755/10.in");
  std::ifstream ans("informatics755/10.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test11)
{
  std::ifstream in("informatics755/11.in");
  std::ifstream ans("informatics755/11.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test12)
{
  std::ifstream in("informatics755/12.in");
  std::ifstream ans("informatics755/12.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test13)
{
  std::ifstream in("informatics755/13.in");
  std::ifstream ans("informatics755/13.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test14)
{
  std::ifstream in("informatics755/14.in");
  std::ifstream ans("informatics755/14.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test15)
{
  std::ifstream in("informatics755/15.in");
  std::ifstream ans("informatics755/15.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}
TEST(informatics755, test16)
{
  std::ifstream in("informatics755/16.in");
  std::ifstream ans("informatics755/16.out");
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  int N;
  in >> N;
  for (int i = 0; i < N; ++i)
  {
    int cmd;
    in >> cmd;
    if (cmd == 0)
    {
      int val;
      in >> val;
      heap.insert(val);
    }
    else
    {
      int val;
      ans >> val;
      EXPECT_EQ(val, heap.top());
      EXPECT_EQ(heap.pop(), 1);
    }
  }
  in.close();
  ans.close();
}