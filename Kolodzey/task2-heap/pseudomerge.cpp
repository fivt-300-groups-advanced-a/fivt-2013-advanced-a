#include <memory>
#include <list>
#include <fstream>
#include "gtest/gtest.h"
#include "biheap.h"
#include "access.h"

void set_level(std::unique_ptr<BiTree<int> >& tree, int level)
{
	TestAccess<BiTree<int>,int> access_tree;
	access_tree._bitree = tree.get();
	access_tree._level() = level;
}
TEST(BiHeap, eat1)
{
	std::ifstream in("mergin/01.in");
	int nRequest;
	in >> nRequest;
  BiHeap<int> heap1;
  TestAccess<BiHeap<int>,int> access_heap1;
  access_heap1._biheap = &heap1;
  
  BiHeap<int> heap2;
  TestAccess<BiHeap<int>,int> access_heap2;
  access_heap2._biheap = &heap2;

	for (int i = 0; i < nRequest; ++i)
	{
		unsigned int heap1description;
		unsigned int heap2description;
		in >> heap1description >> heap2description;
    unsigned int mergeDescription = heap1description + heap2description;
 //   std::cerr << "params of size " << heap1description << " "
 //             << heap2description << " " << mergeDescription << std::endl;
		for (int j = 1; heap1description != 0; heap1description >>= 1, ++j)
    {
      if ((heap1description & 1) != 0)
      {
        std::unique_ptr<BiTree<int> > tree (new BiTree<int> (j));
        set_level(tree, j);
        access_heap1._forest().push_back(nullptr);
        swap(access_heap1._forest().back(), tree);
      }
    }
    for (int j = 1; heap2description != 0; heap2description >>= 1, ++j)
    {
      if ((heap2description & 1) != 0)
      {
        std::unique_ptr<BiTree<int> > tree (new BiTree<int> (j));
        set_level(tree, j);
        access_heap2._forest().push_back(nullptr);
        swap(access_heap2._forest().back(), tree);
      }
    }
    heap1.eat(heap2);
    std::list<std::unique_ptr<BiTree<int> > >::iterator it;
    it = access_heap1._forest().begin();
    for (int j = 1; mergeDescription != 0; mergeDescription >>= 1, ++j)
    {
      if ((mergeDescription & 1) != 0)
      {
        EXPECT_EQ((*it)->get_level(), j);
  //      std::cerr << j << std::endl;
        ++it;
      }
    }
    EXPECT_EQ(it, access_heap1._forest().end());
    heap1.clear();
	}
	in.close();
}

TEST(BiHeap, eat2)
{
  std::ifstream in("mergin/02.in");
  int nRequest;
  in >> nRequest;
  BiHeap<int> heap1;
  TestAccess<BiHeap<int>,int> access_heap1;
  access_heap1._biheap = &heap1;
  
  BiHeap<int> heap2;
  TestAccess<BiHeap<int>,int> access_heap2;
  access_heap2._biheap = &heap2;

  for (int i = 0; i < nRequest; ++i)
  {
    unsigned int heap1description;
    unsigned int heap2description;
    in >> heap1description >> heap2description;
    unsigned int mergeDescription = heap1description + heap2description;
//    std::cerr << "params of size " << heap1description << " "
//              << heap2description << " " << mergeDescription << std::endl;
    for (int j = 1; heap1description != 0; heap1description >>= 1, ++j)
    {
      if ((heap1description & 1) != 0)
      {
        std::unique_ptr<BiTree<int> > tree (new BiTree<int> (j));
        set_level(tree, j);
        access_heap1._forest().push_back(nullptr);
        swap(access_heap1._forest().back(), tree);
      }
    }
    for (int j = 1; heap2description != 0; heap2description >>= 1, ++j)
    {
      if ((heap2description & 1) != 0)
      {
        std::unique_ptr<BiTree<int> > tree (new BiTree<int> (j));
        set_level(tree, j);
        access_heap2._forest().push_back(nullptr);
        swap(access_heap2._forest().back(), tree);
      }
    }
    heap1.eat(heap2);
    std::list<std::unique_ptr<BiTree<int> > >::iterator it;
    it = access_heap1._forest().begin();
    for (int j = 1; mergeDescription != 0; mergeDescription >>= 1, ++j)
    {
      if ((mergeDescription & 1) != 0)
      {
        EXPECT_EQ((*it)->get_level(), j);
 //       std::cerr << j << std::endl;
        ++it;
      }
    }
    EXPECT_EQ(it, access_heap1._forest().end());
    heap1.clear();
  }
  in.close();
}