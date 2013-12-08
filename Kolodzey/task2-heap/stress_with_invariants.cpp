#include <fstream>
#include "gtest/gtest.h"
#include "biheap.h"
#include "access.h"
#include "checkinvariants.h"

//Testing check invariants functions
TEST(Small, heapness_of_tree)
{
  std::unique_ptr<BiTree<int> > a (new BiTree<int> (1));
  std::unique_ptr<BiTree<int> > b (new BiTree<int> (2));
  std::unique_ptr<BiTree<int> > c (new BiTree<int> (3));
  std::unique_ptr<BiTree<int> > d (new BiTree<int> (4));
  std::unique_ptr<BiTree<int> > e (new BiTree<int> (5));
  std::unique_ptr<BiTree<int> > f (new BiTree<int> (6));
  std::unique_ptr<BiTree<int> > g (new BiTree<int> (7));
  std::unique_ptr<BiTree<int> > h (new BiTree<int> (8));
  a->eat(b);
  c->eat(d);
  e->eat(f);
  g->eat(h);
  a->eat(c);
  e->eat(g);
  a->eat(e);
  checkHeapnessTree<int, std::less<int> >(a, (std::less<int>()));
}

TEST(Small, heapness)
{
  BiHeap<int> heap;
  heap.insert(5);
  heap.insert(1);
  checkHeapnessHeap<int, std::less<int> >(heap);
}

TEST(Small, ChildAndLevel)
{
  std::unique_ptr<BiTree<int> > a (new BiTree<int> (1));
  std::unique_ptr<BiTree<int> > b (new BiTree<int> (2));
  std::unique_ptr<BiTree<int> > c (new BiTree<int> (3));
  std::unique_ptr<BiTree<int> > d (new BiTree<int> (4));
  std::unique_ptr<BiTree<int> > e (new BiTree<int> (5));
  std::unique_ptr<BiTree<int> > f (new BiTree<int> (6));
  std::unique_ptr<BiTree<int> > g (new BiTree<int> (7));
  std::unique_ptr<BiTree<int> > h (new BiTree<int> (8));
  a->eat(b);
  c->eat(d);
  e->eat(f);
  g->eat(h);
  a->eat(c);
  e->eat(g);
  a->eat(e);
  checkChildAndLevel<int>(a);
}

TEST(Small, childAndLevelHeap)
{
  BiHeap<int> heap;
  heap.insert(5);
  heap.insert(1);
  heap.insert(8);
  heap.insert(6);
  heap.insert(7);
  checkChildAndLevelHeap<int, std::less<int> >(heap);
}

TEST(Small, checkRefsToParent)
{
  std::unique_ptr<BiTree<int> > a (new BiTree<int> (1));
  std::unique_ptr<BiTree<int> > b (new BiTree<int> (2));
  std::unique_ptr<BiTree<int> > c (new BiTree<int> (3));
  std::unique_ptr<BiTree<int> > d (new BiTree<int> (4));
  std::unique_ptr<BiTree<int> > e (new BiTree<int> (5));
  std::unique_ptr<BiTree<int> > f (new BiTree<int> (6));
  std::unique_ptr<BiTree<int> > g (new BiTree<int> (7));
  std::unique_ptr<BiTree<int> > h (new BiTree<int> (8));
  a->eat(b);
  c->eat(d);
  e->eat(f);
  g->eat(h);
  a->eat(c);
  e->eat(g);
  a->eat(e);
  checkRefsToParent<int>(a,nullptr);
}

TEST(Small, checkRefsToParentHeap)
{
  BiHeap<int> heap;
  heap.insert(5);
  heap.insert(1);
  heap.insert(8);
  heap.insert(6);
  heap.insert(7);
  checkRefsToParentHeap<int, std::less<int> >(heap);
}

TEST(Small, checkSize)
{
  BiHeap<int> heap;
  heap.insert(5);
  heap.insert(1);
  heap.insert(8);
  heap.insert(6);
  heap.insert(7);
  checkSize<int, std::less<int> >(heap);
}

//Stress!!!

TEST(EatPopInsert, test1)
{
  const int N_OF_HEAPS = 1000;
  const int LOG_NUMBER = 5000;
  BiHeap<int> heap[N_OF_HEAPS];
  for (int i = 0; i < N_OF_HEAPS; ++i)
  {
    heap[i].clear();
    EXPECT_EQ(heap[i].size(), 0);
  }
  std::ifstream in("eatpopinsert/01.in");
  int cmd = -1;
  int nrequest = 0;
  do
  {
    in >> cmd;
    int p, p1, p2, x;
    switch(cmd)
    {
      case 1:
        in >> p >> x;
        heap[p].insert(x);
        break;
      case 2:
        in >> p;
        heap[p].pop();
        break;
      case 3:
        in >> p1 >> p2;
        heap[p1].eat(heap[p2]);
        break;
    }
    ++nrequest;
    if (nrequest % LOG_NUMBER == 0)
    {
      for (int i = 0; i < N_OF_HEAPS; ++i)
        checkAllInvariants<int, std::less<int> >(heap[i]);
      std::cerr << "passed " << nrequest << " requests" << std::endl;
    }
  }
  while (cmd != -1);
  for (int i = 0; i < N_OF_HEAPS; ++i)
  {
    heap[i].clear();
    EXPECT_EQ(heap[i].size(), 0);
  }
}

TEST(EatPopInsert, test2)
{
  //std::cerr << "entered test2" << std::endl;
  const int N_OF_HEAPS = 10;
  const int LOG_NUMBER = 1;
  BiHeap<int> heap[N_OF_HEAPS];//при инициализации массива в кучах мусор
  for (int i = 0; i < N_OF_HEAPS; ++i)
  {
    heap[i].clear();
    EXPECT_EQ(heap[i].size(), 0);
  }
  //std::cerr << "cleared heaps" << std::endl;
  std::ifstream in("eatpopinsert/02.in");
  int cmd = -1;
  int nrequest = 0;
  do
  {
    in >> cmd;
    int p, p1, p2, x;
    switch(cmd)
    {
      case 1:
        in >> p >> x;
   //     std::cerr << "heap[" << p << "].insert(" << x << ");" << std::endl;
        heap[p].insert(x);
        break;
      case 2:
        in >> p;
        heap[p].pop();
    //    std::cerr << "heap[" << p << "].pop( );" << std::endl;
        break;
      case 3:
        in >> p1 >> p2;
        heap[p1].eat(heap[p2]);
    //    std::cerr << "heap[" << p1 << "].eat(heap[" << p2 << "]);" << std::endl;
        break;
    }
    ++nrequest;
    if (nrequest % LOG_NUMBER == 0)
    {
      for (int i = 0; i < N_OF_HEAPS; ++i)
      {
      //  std::cerr << "I'm checking heap #" << i << std::endl;
        checkAllInvariants<int, std::less<int> >(heap[i]);
      }
    }
  }
  while (cmd != -1);
  for (int i = 0; i < N_OF_HEAPS; ++i)
  {
    heap[i].clear();
    EXPECT_EQ(heap[i].size(), 0);
  }
}

TEST(EatPopInsert, test3)
{
  const int N_OF_HEAPS = 100;
  const int LOG_NUMBER = 5000;
  BiHeap<int> heap[N_OF_HEAPS];
  for (int i = 0; i < N_OF_HEAPS; ++i)
  {
    heap[i].clear();
    EXPECT_EQ(heap[i].size(), 0);
  }
  std::cerr << "cleared heaps" << std::endl;
  std::ifstream in("eatpopinsert/03.in");
  int cmd = -1;
  int nrequest = 0;
  do
  {
    in >> cmd;
    int p, p1, p2, x;
    switch(cmd)
    {
      case 1:
        in >> p >> x;
        heap[p].insert(x);
        break;
      case 2:
        in >> p;
        heap[p].pop();
        break;
      case 3:
        in >> p1 >> p2;
        heap[p1].eat(heap[p2]);
        break;
    }
    ++nrequest;
    if (nrequest % LOG_NUMBER == 0)
    {
      for (int i = 0; i < N_OF_HEAPS; ++i)
        checkAllInvariants<int, std::less<int> >(heap[i]);
      std::cerr << "passed " << nrequest << " requests" << std::endl;
    }
  }
  while (cmd != -1);
  for (int i = 0; i < N_OF_HEAPS; ++i)
  {
    heap[i].clear();
    EXPECT_EQ(heap[i].size(), 0);
  }
}